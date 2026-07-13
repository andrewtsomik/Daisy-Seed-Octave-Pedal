#include "daisy_seed.h"
#include "daisysp.h"
 
using namespace daisy;
using namespace daisy::seed;
using namespace daisysp;
 
#define POT1 A0
#define POT2 A1
#define POT3 A2
#define LED1  D19
#define FS_SWITCH  D18
 
enum AdcChannel
{
	PotOne,
	PotTwo,
	PotThree,
	NUM_ADC_CHANNELS
};
 
DaisySeed hw;
 
PitchShifter sub_shift;
PitchShifter upper_shift;
OnePole env_lpf;
Led led_gate;
Switch footswitch;
 
static constexpr float ENV_THRESHOLD_OPEN  = 0.01f;
static constexpr float ENV_THRESHOLD_CLOSE = 0.006f;
static constexpr float GATE_SMOOTH_COEFF   = 0.01f;
static constexpr float SMOOTH              = 0.002f;
 
float dry_smooth   = 0.7f;
float sub_smooth   = 0.8f;
float upper_smooth = 0.0f;
 
static bool  gate_state  = false;
static float gate_smooth = 0.0f;
 
static bool effect_on         = false;
static bool footswitch_last   = false;
 
void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	bool footswitch_pressed = footswitch.Pressed();
	if(footswitch_pressed && !footswitch_last)
	{
		effect_on = !effect_on;
	}
	footswitch_last = footswitch_pressed;
 
	float pot_dry   = hw.adc.GetFloat(0);
	float pot_sub   = hw.adc.GetFloat(1);
	float pot_upper = hw.adc.GetFloat(2);
 
	dry_smooth   += SMOOTH * (pot_dry - dry_smooth);
	sub_smooth   += SMOOTH * (pot_sub - sub_smooth);
	upper_smooth += SMOOTH * (pot_upper - upper_smooth);
 
	for(size_t i = 0; i < size; i++)
	{
		float input = in[0][i];
 
		float rectified = fabsf(input);
		float envelope   = env_lpf.Process(rectified);
 
		if(!gate_state && envelope > ENV_THRESHOLD_OPEN)
		{
			gate_state = true;
		}
		else if(gate_state && envelope < ENV_THRESHOLD_CLOSE)
		{
			gate_state = false;
		}
 
		float gate_target = gate_state ? 1.0f : 0.0f;
		gate_smooth += GATE_SMOOTH_COEFF * (gate_target - gate_smooth);
 
		led_gate.Set(fclamp(envelope * 8.0f, 0.0f, 1.0f));
 
		float sub_out   = sub_shift.Process(input) * gate_smooth;
		float upper_out = upper_shift.Process(input) * gate_smooth;
 
		float mixed;
		if(effect_on)
		{
			mixed = input * dry_smooth + sub_out * sub_smooth + upper_out * upper_smooth;
			mixed = mixed / (1.0f + fabsf(mixed));
		}
		else
		{
			mixed = input;
		}
 
		out[0][i] = mixed;
		out[1][i] = mixed;
	}
}
 
int main(void)
{
	hw.Init();
	hw.SetAudioBlockSize(4);
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
 
	float sample_rate = hw.AudioSampleRate();
 
	AdcChannelConfig adc_cfg[NUM_ADC_CHANNELS];
	adc_cfg[PotOne].InitSingle(POT1);
	adc_cfg[PotTwo].InitSingle(POT2);
	adc_cfg[PotThree].InitSingle(POT3);
	hw.adc.Init(adc_cfg, NUM_ADC_CHANNELS);
	hw.adc.Start();
 
	sub_shift.Init(sample_rate);
	sub_shift.SetTransposition(-12.0f);
	sub_shift.SetFun(0.0f);
 
	upper_shift.Init(sample_rate);
	upper_shift.SetTransposition(12.0f);
	upper_shift.SetFun(0.0f);
 
	env_lpf.Init();
	env_lpf.SetFrequency(20.0f / sample_rate);
 
	led_gate.Init(LED1, false);
 
	footswitch.Init(FS_SWITCH, 1000.0f, Switch::TYPE_MOMENTARY, Switch::POLARITY_INVERTED, Switch::PULL_UP);
 
	hw.StartAudio(AudioCallback);
 
	while(1)
	{
		footswitch.Debounce();
		led_gate.Update();
		System::Delay(1);
	}
}