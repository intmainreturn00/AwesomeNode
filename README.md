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
