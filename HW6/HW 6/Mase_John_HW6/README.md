# Name: John (JJ) Mase
# USC ID: 6904908117
# Email: mase@usc.edu
# Visual Studio Version: 16.11.0
# Other Information
## Example Outputs
### Textures
Textures can be enabled through the ```#if```, ```#elif```, and ```#else``` compiler directives on line 175-180 of Application6.cpp.  
```#if 1``` will set the texture to the procedural texture, ```#elif 1``` will set the texture to the given USC texture, and ```#else``` will not render with a texture.

### Lighting/Shading
Gouraud Shading can be enabled through line 162  
Phong Shading can be enabled through line 163

## Architecture
Unlike the example code architecture given, I've opted to incorporate the multiple, shifted renderers into the GzRender object itself. Thus, no parameters need to flow through GzPutAttribute or the application driver and instead the anti-aliasing is driven by the shape/values within the anti-aliasing kernel and the ```#define AAKERNEL_SIZE```. Accordingly, the ```GZ_AASHIFTX``` and ```GZ_AASHIFTY``` parameters have been omitted GzPutAttribute. Accordingly to the Slack channel, the code implementation should not affect the grade, therefore I've structured the assignment this way.  

## Render Time
Since we're now sampling 6x as many pixels, the 256 x 256 image now takes a few seconds to render.