# awesome node
Cocos2d-x tessellation library for drawing smooth anti-aliased curves.

Make drawing great again!
![](awesome%20node/2019-03-21%2004.50.23.jpg)

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
<img src="https://img.shields.io/badge/made%20with-cocos2dx-blue.svg" alt="made with Cocos2dx">
![GitHub stars](https://img.shields.io/github/stars/intmainreturn00/AwesomeNode.svg?style=social)

You can read about implementation details at my [Medium Post.](https://medium.com/@intmainreturn00/drawing-smooth-lines-with-cocos2dx-d057c41b8af2)
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


# Future plans
I’ve also planned to add the ability to do color and thickness gradients in the future if someone needs it.

[Metal-backend](http://discuss.cocos2d-x.org/t/progress-of-graphics-backend-integration/44671/32) currently doesn't work

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
