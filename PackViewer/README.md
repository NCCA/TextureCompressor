# PackViewer
![alt tag](http://nccastaff.bournemouth.ac.uk/jmacey/GraphicsLib/Demos/TextureComp.png)

This program reads the textures loaded into the .pack format created in the PackTool demo. Use the arrow keys to move between the different files

Note that the texture once stored is in BGR format so reading in the shader should look like this

```c++
  fragColour.rgb=texture(tex,vertUV ).bgr;
```