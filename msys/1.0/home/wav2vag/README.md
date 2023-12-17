# Aikku93's wav2vag
WAV to VAG (PSX format) conversion tool.

Because there is enough of these floating around on the internet, this one is known as "Aikku93's wav2vag".

## Getting started

### Prerequisites
Compile with GCC, Clang, or whatever C compiler you like. Some may throw fits over hexadecimal floats and other nonstandard stuff I've used.

### Installing
Run ```make```. You might have to tweak the Makefile, as it's currently configured to cross-compiling into Windows x86.

## Usage
To paraphase the usage displayed when called with no arguments:

```wav2vag Input.wav Output.vag [-l:Begin[-End]] [-r:1.0] [-rt:sinc] [-name:[blank]]```

This takes ```Input.wav``` and creates ```Output.vag```. ```-l``` sets manual loop points (ie. overrides any loops found in the file), ```-r``` allows a resampling factor (eg. 0.5 will halve the sampling rate), ```-rt``` sets the resampling mode (```lin```, ```cubic``` (Catmull-Rom), or ```sinc``` (Nuttall-windowed sinc)), and ```-name``` sets the internal name in the VAG header.

For a more complete description, run the program without any arguments and read the usage/options.

## Possible issues
* Data past the loop-end point is trimmed off and NOT encoded. This can break compatibility with sampled releases (assuming anyone uses these).
* Although I've tried to idiot-proof things, this tool is not a replacement for a brain. Ridiculous-but-valid parameters might break things.
* Some SPUADPCM decoders don't appear to handle clipping correctly, leading to cracks/pops/corrupt output.
    * Some decoders are also using floating-point maths for decoding (WHAT?!), potentially causing DC drift on playback due to encoding/decoding mismatches.
    * Additionally, no-one seems to agree on how to handle the linear prediction filter (floating point, shift right after each multiply, rounding behaviour, etc. are all different)
    * Also, some decoders are NOT interpreting single-frame (28-sample) loops correctly and seem to think that they're silence-padding for one-shot samples?
    * And on that note, can we PLEASE standardize the way that the PSX's SPUADPCM is decoded? Everyone appears to have their own take on how things should be done instead of referring to good documentations (eg. no$psx)

## Authors
* **Ruben Nunez** - *Initial work* - [Aikku93](https://github.com/Aikku93)

## License
Aikku93's wav2vag is released under the GPLv3 license. See the LICENSE file for full terms.

## Acknowledgements
* Thanks to `musicalman` for asking to see my take on a wav2vag conversion tool. It was... insightful, if nothing else.
