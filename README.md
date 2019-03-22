# awesome node
Cocos2d-x tessellation library for drawing smooth anti-aliased curves.

Make drawing great again!
![](awesome%20node/2019-03-21%2004.50.23.jpg)

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
<img src="https://img.shields.io/badge/made%20with-kotlin-blue.svg" alt="made with Cocos2dx">

If you’d like the work, star this repo ⭐️ or you can [buy me a cup of coffee](http://ko-fi.com/intmainreturn00) ☕️

# Setup
Just grab `AwesomeNode.h` and `AwesomeNode.cpp` into your cocos2d-x based project and you’re all set! (`AwesomeNode` don’t expose any cocos namespaces to global scope).

Don’t forget to add `AwesomeNode.cpp` to `LOCAL_SRC_FILES` section
at `Android.mk` on android. Projects for Android Studio and MSVC are also provided.

# Usage
Initialise the node and add it to scene..
```cpp
auto node = AwesomeNode::create();
addChild(node);
```
…and you can start drawing.

Given that `w` - is the line thickness you want and `pts` is cocos `PointArray` you can do the following:

```cpp
drawALine(A, B, w, Color4F::GREEN);
node->drawACardinalSpline(pts, 0.5, 360, w, Color4F::GREEN);
node->drawAFilledCardinalSpline(pts, 0.5, 360, w, Color4F::GREEN, bottom, Color4F::RED);
```

last one generate curve with color filling from curve to the given Y level, like you can see at sample. 

Also you can draw a triangle (`drawTriangle`) with each vertex can has its own color and opacity so OpenGL can provide color interpolation at vertex shader. Tessellation is basically a smart way to represent line by a set of triangles so this one is used all across the library. 

Simple implementation for dashed (`drawADashedLine`) and dash-dotted (`drawADashDottedLine`) lines are also included.

Uncomment `#define AWESOMEDEBUG` to see tessellation highlights. 
<img src="awesome%20node/1AD38AD9-90C0-4973-B5A2-8DD9F018231A.png" width="500">

# Motivation
It’s all started from my humble attempts to draw some lines with cocos2d-x. 
You can partly track down my research after that from [this topic](https://discuss.cocos2d-x.org/t/final-topic-about-anti-aliasing-disease/37666) at cocos forum. The post currently gathered more that 3.4K views so I think the problem is quite common. 

OpenGL is lacking ability to draw curves with arbitrary thickness and even with different antialiasing techniques, which is more affordable for desktops rather than for mobile devices, can’t get the work done. In native development we can use [Skia](https://skia.org/), which [implement](https://github.com/google/skia/blob/master/src/core/SkScan_AAAPath.cpp) tricky AA solution for paths, but cocos2d-x drawing primitives dose not include such functionality. You can read more about different approaches at the Link section below.

With standard `DrawNode`we can draw something like this:
```cpp
node = DrawNode::create(9);
node->drawCardinalSpline(pts, tension, segments, Color4F::RED);
for (int i = 0; i < pts->count(); ++i) {
    node->drawPoint(pts->getControlPointAtIndex(i), 10, Color4F::BLUE);
}
```

<img src="awesome%20node/5BC656C4-EB97-4B0D-8B8C-4D442102EBF1.png" width="500">
this picture has following artefacts:
1. stepped curve borders (aliasing)
2. some cut holes inside line.

Varying the number of segments or draw-then-scale can eliminate only small fraction of all the artefacts, mostly because the problem is not exactly in smoothing the line edges but rather in correct representation of it with a set of triangles - tessellation. 

After I slightly modify `DrawNode`, adding the ability to draw a triangles with each vertex having its own color I’we been able to do something like this:
![](awesome%20node/A0C97041-7160-4469-AC47-7CB17C01E7F9.png)
This curve was drawn by setting up the coordinates and colours for every one triangle vertices, letting OpenGL do color interpolation, making gradient to zero opacity, which results in feather disappearing . This looks solid, but do I really need engine, if I do triangle drawing all by myself?

I could link to libskia on android, make some path objects on skia side (skia will do tessellation and anti-aliasing), export them to bitmap and then draw with cocos. But again… why do I need cocos in that case?

Playing with shaders and DrawNode is terra incognito (If you know at least one non trivial and at least compilable solution, please, send me a link). After some experiments I’ve found the only one thing which cocos draws strangely smooth - segment. 
![](awesome%20node/A066FE00-DEC3-492A-8810-388AB795CF06.png)
I highlight geometry for you. As you can see, it is drawn from 6 triangles and all the magic happens at this line of the fragment shader:
```GLSL
gl_FragColor = v_color*smoothstep(0.0, 0.1, 1.0 - length(v_texcoord));
```
when DrawNode sets the texture offsets at `drawSegment`.

We can reuse such primitive inside cardinal splines and draw curves with segments instead of lines:
![](awesome%20node/A7B94B8C-0299-4BC7-8B0C-120650A875E7.png)
![](awesome%20node/2509EAD6-D234-4231-8053-B9DE00928E53.png)
I’ve also fix the [bug at cardinal splines math](https://discuss.cocos2d-x.org/t/catmull-rom-cardinal-spline-interpolation-problem/4586), which is apparently been there since the beginning of `DrawNode`.

If you zoom up this images, you would see that at large widths we’ve got good results, but at thin lines there are still some artefacts. 

For such cases I’ve implemented simple tessellation algorithm:
![](awesome%20node/DCC201A2-AA36-48FC-BE6E-A58B22410711.png)
I used 4 triangles for each line segment and set the border vertices opacity to zero. Such scheme doesn’t work well with wide lines - we need more triangle layers for this, but fortunately enough I only need this for thin lines:
![](awesome%20node/D25A1099-15BC-46EE-AA68-BB4DBBF7675C.png)
![](awesome%20node/BEB6F690-4575-45F0-AD8D-AADAFD16ADB0.png)

Combining both approach we’ll have the results you can see at the header of this post. 

# Future plans
I’ve also planned to add the ability to do color and thickness gradients in the future if someone needs it.

# Links
* [Drawing Lines is Hard](https://mattdesl.svbtle.com/drawing-lines-is-hard)
* [Drawing Antialiased Lines with OpenGL](https://blog.mapbox.com/drawing-antialiased-lines-with-opengl-8766f34192dc)
* [Vase](http://tyt2y3.github.io/vaser-web/) - tried. Even with pretty complex geometry can’t solve [some issues](https://discuss.cocos2d-x.org/uploads/default/original/3X/6/9/693b2c2f5d6282618d53e7a6900a2fb68c463089.png)
* [Antialiasing](http://discuss.cocos2d-x.org/t/antialiasing/1360/3) . Suggestion to use open gl attribute which as far as I know exist only at desktop. not in mobile devices. and link, which don’t exist anymore.
* [CCDrawNode Antialiasing problem](http://discuss.cocos2d-x.org/t/ccdrawnode-antialiasing-problem/7916/3)  all the mentioned previously methods (CCRenderTexture, smoothstep, etc). has very small effect. Maybe you can handle just lines with it, but not curves or circles.
* [CCDrawNode Antialiasing problem](http://discuss.cocos2d-x.org/t/ccdrawnode-antialiasing-problem/13970)  unanswered. lost. wasted.
* [Cocos2d-x v3 Anti-Aliasing for Primitives](http://discuss.cocos2d-x.org/t/cocos2d-x-v3-anti-aliasing-for-primitives/20548)  same solutions. same results.
* [Draw circle with strong border](http://discuss.cocos2d-x.org/t/draw-circle-with-strong-border/14827)  same problem - no help.
* [DrawNode anti-aliasing solution](http://discuss.cocos2d-x.org/t/drawnode-anti-aliasing-solution/20651)  nope. not a solution. really. or do I do something wrong?
* [How to draw anti-aliased line in cocos2d-x for android games?](http://discuss.cocos2d-x.org/t/how-to-draw-anti-aliased-line-in-cocos2d-x-for-android-games/12872)  suggested again to use not existed at openglES method and then smoothstep. arrrggghhh!
*  [Anti-Grain Geometry (AGG) 49](http://www.antigrain.com/doc/introduction/introduction.agdoc.html#toc0002) Try out its android app. And its aliased at my nexus device! WTF? Antialiased anti-artifacts library demo consist of aliased graphics??! Arggghh.
