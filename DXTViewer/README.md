#DXTViewer
![alt tag](http://nccastaff.bournemouth.ac.uk/jmacey/GraphicsLib/Demos/TextureComp.png)

A simple program to view DXT files, it runs from the command line and show the first command line argument only.

Note that the texture once stored is in BGR format so reading in the shader should look like this

```c++
  fragColour.rgb=texture(tex,vertUV ).bgr;
```