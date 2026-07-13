# Daisy Seed Octave Pedal

Using a Daisy Seed microcontroller, simulate an octave effects pedal for electric bass!
## Features

- **Sub octave** (-12 semitones) and **upper octave** (+12 semitones), each with its own level control
- **Envelope-gated** octave voices with hysteresis, so quiet passages and note decay don't produce gate chatter/zipper noise
- **Latching footswitch** one tap turns the effect on, another tap bypasses it
- Onboard LED tracks your playing envelope in real time
- True bypass fallback: bypassed signal is the untouched dry input
## Hardware
 
| Component        | Daisy Seed Pin | Purpose                     |
|------------------|:--------------:|------------------------------|
| Pot 1            | A0              | Dry level                   |
| Pot 2            | A1              | Sub-octave level             |
| Pot 3            | A2              | Upper-octave level           |
| Footswitch       | D18             | Effect on/off (momentary, wired to pull-up, active low) |
| LED              | D19             | Envelope indicator           |
| Input Jack       | A16             | Bass Input                   |
| Output Jack      | A18             | Amp Output                   |
| DC Jack          | A39             | Power Source                       |

## Schematic

<img width="906" height="766" alt="image" src="https://github.com/user-attachments/assets/e1a0a77d-f986-4e81-a076-1608b0242d73" />

## PCB Layout

<img width="535" height="551" alt="image" src="https://github.com/user-attachments/assets/5f916799-5627-4d49-bacf-b733ff473431" />

## Fabricated and Assembled Design

### Front
<img width="342" height="261" alt="image" src="https://github.com/user-attachments/assets/0a380112-cc9a-4799-a931-266a811d6f41" />

### Back
<img width="342" height="261" alt="image" src="https://github.com/user-attachments/assets/c2ea794a-aebd-4e7f-a3c2-77fef65350f9" />

### Assembled
<img width="342" height="261" alt="image" src="https://github.com/user-attachments/assets/549299a3-46d0-4949-bf8d-d9b3928f44f1" />

## Demo (Don't hate I suck lol)

### Toggled On

https://github.com/user-attachments/assets/edada847-4ac4-451e-820c-bb3a319f9780

### Toggled Off

https://github.com/user-attachments/assets/274790b7-3237-4e18-9de3-0ea1d50deda9

## Flashing Instructions
Pre-Step 1
Run the commands below if you have not already cloned the libraries needed
```bash
git clone --recursive https://github.com/electro-smith/libDaisy.git
git clone --recursive https://github.com/electro-smith/DaisySP.git
```

1. Hold down the BOOT button your Daisy Seed
2. Press and release the RESET button
3. Release the BOOT button
4. Run the following command (Getting Error 74 is normal and a sign everything is working)

```bash
make clean && make && make program-dfu
```

5. Press and release the RESET button again
