# DXTViewer
![alt tag](http://nccastaff.bournemouth.ac.uk/jmacey/GraphicsLib/Demos/TextureComp.png)

A simple program to view DXT files, it runs from the command line and show the first command line argument. If no argument is passed a load dialog will be shown.

To load a new file press o

Note that the texture once stored is in BGR format so reading in the shader should look like this

```c++
  fragColour.rgb=texture(tex,vertUV ).bgr;
```