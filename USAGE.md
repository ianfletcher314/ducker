# Ducker - Usage Guide

**Sidechain Ducker**

Ducker is a precision sidechain ducking plugin designed for creating space in your mix by automatically lowering one element when another plays. Perfect for the classic kick/bass relationship, vocal ducking, and creative pumping effects.

---

## Use Cases in Modern Rock Production

### Drum Bus Processing

The drum bus itself typically doesn't need ducking, but drums often trigger ducking on other elements.

**Kick as Sidechain Source:**
- Send your kick drum to Ducker's sidechain input
- Use to duck bass, rhythm guitars, or synths
- Creates pocket for kick to punch through

### Guitar Bus / Individual Tracks

Guitar ducking creates space for vocals or drums.

**Duck Guitars for Vocals:**
- Sidechain: Lead vocal
- Threshold: -18 to -24 dB
- Duck Amount: -3 to -6 dB
- Attack: 15-25 ms
- Hold: 50-100 ms
- Release: 150-250 ms
- Curve: S-Curve (smooth)
- SC HPF: 200 Hz (ignore low vocal content)

**Duck Guitars for Kick:**
- Sidechain: Kick drum
- Threshold: -20 dB
- Duck Amount: -2 to -4 dB
- Attack: 5-10 ms
- Hold: 20-50 ms
- Release: 100-150 ms
- Curve: Linear or Exponential

### Bass Guitar

Bass ducking is essential for a tight low end in modern rock.

**Classic Kick/Bass Ducking:**
- Sidechain: Kick drum
- Threshold: -15 to -20 dB
- Duck Amount: -4 to -8 dB
- Attack: 1-5 ms (fast)
- Hold: 30-50 ms
- Release: 100-150 ms
- Curve: Exponential
- SC HPF: Off (want full kick)
- SC LPF: 200 Hz (focus on kick fundamental)
- Range: -20 dB (limit maximum ducking)

**Subtle Bass Pocket:**
- Sidechain: Kick drum
- Threshold: -18 dB
- Duck Amount: -2 to -3 dB
- Attack: 5-10 ms
- Hold: 20 ms
- Release: 80-120 ms
- Curve: S-Curve
- Mix: 80%

**Aggressive Pumping Bass:**
- Sidechain: Kick drum
- Threshold: -25 dB
- Duck Amount: -12 to -20 dB
- Attack: 1 ms
- Hold: 50-100 ms
- Release: 200-400 ms (tempo-synced if available)
- Curve: Linear

### Vocals

Ducking elements under vocals ensures clarity.

**Duck Instruments for Lead Vocal:**
- Insert Ducker on guitar bus, synth bus, or full mix minus vocals
- Sidechain: Lead vocal
- Threshold: -20 dB
- Duck Amount: -2 to -4 dB
- Attack: 20-30 ms
- Hold: 50 ms
- Release: 200 ms
- Curve: S-Curve
- SC HPF: 150-200 Hz (ignore breath sounds)

**Duck Reverb for Vocal Clarity:**
- Insert Ducker on vocal reverb return
- Sidechain: Dry lead vocal
- Threshold: -18 dB
- Duck Amount: -6 to -12 dB
- Attack: 10 ms
- Hold: 20 ms
- Release: 300-500 ms
- Curve: Logarithmic (gentle return)

### Mix Bus / Mastering

**Sidechain Compression Alternative:**
- Insert on mix bus (minus kick)
- Sidechain: Kick drum
- Threshold: -20 dB
- Duck Amount: -1 to -2 dB
- Attack: 5 ms
- Hold: 20 ms
- Release: 80 ms
- Creates subtle pumping/breathing effect

---

## Recommended Settings

### Quick Reference Table

| Application | Threshold | Duck Amount | Attack | Hold | Release | Curve |
|------------|-----------|-------------|--------|------|---------|-------|
| Kick/Bass (tight) | -18 dB | -6 dB | 3 ms | 40 ms | 120 ms | Exp |
| Kick/Bass (pumping) | -25 dB | -15 dB | 1 ms | 80 ms | 300 ms | Linear |
| Guitars for Vocal | -20 dB | -4 dB | 20 ms | 60 ms | 200 ms | S-Curve |
| Reverb Duck | -18 dB | -8 dB | 10 ms | 20 ms | 400 ms | Log |
| Mix Pump | -20 dB | -2 dB | 5 ms | 20 ms | 80 ms | S-Curve |

### Curve Shape Guide

- **Linear**: Straightforward ducking - predictable, neutral character
- **Exponential**: Fast initial drop, gradual return - tight, punchy feel
- **Logarithmic**: Gentle drop, fast return - smooth, natural ducking
- **S-Curve**: Smooth both ways - most transparent, least noticeable

### Sidechain Filter Settings

**SC HPF (High-Pass Filter):**
- **Off**: Full frequency range triggers ducking
- **60-80 Hz**: Remove sub-bass rumble from triggering
- **150-200 Hz**: Focus on mid-range content (for vocal sidechaining)
- **300+ Hz**: Only upper frequencies trigger (special effects)

**SC LPF (Low-Pass Filter):**
- **200 Hz**: Only low frequencies trigger (classic kick ducking)
- **1000 Hz**: Balanced triggering
- **5000+ Hz**: Include high-frequency transients

---

## Signal Flow Tips

### Where to Place Ducker

1. **On the Track Being Ducked**: Insert Ducker on bass, guitars, or the element you want to reduce
2. **Sidechain from Source**: Route kick, vocal, or trigger source to Ducker's sidechain input

### Setting Up Sidechain Routing

**In most DAWs:**
1. Create a send from your trigger source (kick, vocal)
2. Or use your DAW's sidechain routing to select the input
3. Ducker accepts both dedicated sidechain input and the same input (for self-ducking)

### Using Look-Ahead

- **0 ms**: No latency, natural response
- **5-10 ms**: Catches transients before they hit, tighter ducking
- **15-20 ms**: Maximum anticipation, very precise but adds latency
- **Note**: Look-ahead adds latency to your project - compensate or use sparingly

### Using Zero-Crossing

- Reduces clicks when ducking happens
- Slight delay in response
- Best for longer attack times
- Disable for fastest transient response

### Tempo Sync

When enabled:
- Hold and Release sync to your project tempo
- Use musical note divisions (1/8, 1/4, etc.)
- Creates rhythmic pumping effects
- Great for EDM-style sidechain in rock contexts

---

## Combining with Other Plugins

### Kick/Bass Relationship
1. **Bus Glue** on bass (compression)
2. **Ducker** on bass (sidechained from kick)
3. Result: Tight, punchy low end

### Vocal Clarity Chain
1. **VoxProc** on lead vocal
2. **Ducker** on guitar/synth bus (sidechained from vocal)
3. Result: Vocals always sit on top

### Creative Pumping
1. **Ducker** on synth/pad (sidechained from kick)
2. **TapeWarm** after (warm up the pump)
3. Result: Classic EDM-style pump with rock warmth

### Reverb Ducking Chain
1. **Dynoverb** or reverb send
2. **Ducker** on reverb return (sidechained from dry signal)
3. Result: Reverb that doesn't mask the source

---

## Quick Start Guide

**Set up kick/bass ducking in 30 seconds:**

1. Insert Ducker on your bass track
2. Set up sidechain input from kick drum
3. Set **Threshold** to -18 dB
4. Set **Duck Amount** to -6 dB
5. Set **Attack** to 3 ms
6. Set **Hold** to 40 ms
7. Set **Release** to 120 ms
8. Set **Curve** to Exponential
9. Enable **SC LPF** at 200 Hz
10. Play kick and bass together - bass should dip with each kick hit
11. Adjust Duck Amount until kick punches through without bass disappearing

**Set up vocal ducking in 30 seconds:**

1. Insert Ducker on your guitar bus (or instrument you want to duck)
2. Set up sidechain input from lead vocal
3. Set **Threshold** to -20 dB
4. Set **Duck Amount** to -3 dB
5. Set **Attack** to 20 ms
6. Set **Hold** to 50 ms
7. Set **Release** to 200 ms
8. Set **Curve** to S-Curve
9. Enable **SC HPF** at 200 Hz
10. Play vocal with guitars - guitars should subtly dip when vocal enters
11. Fine-tune Duck Amount so it's felt but not heard

**Create pumping effect in 30 seconds:**

1. Insert Ducker on pad/synth/guitar track
2. Set up sidechain from kick drum
3. Set **Threshold** to -25 dB
4. Set **Duck Amount** to -15 dB
5. Set **Attack** to 1 ms
6. Enable **Tempo Sync**
7. Set **Hold Sync** to 1/16
8. Set **Release Sync** to 1/8 or 1/4
9. Set **Curve** to Linear
10. Play and adjust Release Sync for desired pump length
