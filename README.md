# RhinoBrush
A Rhino Plugin for NURBS Editing


我们的代码集中在以下三个文件中：

Our contribution is mainly contained in those three documents below:

**RhinoBrushClass.h**		（类声明与函数声明头文件）

**RhinoBrushClass.cpp**	（类成员函数和类外函数实现）

**cmdRhinoBrush.cpp**	  （插件执行主函数文件）

最终生成的插件为：
the final Rhino plugin is:

**RhinoBrush.rhp**		（需Rhino7正式版本，具体使用参考用户手册）

rhino开发者文档提供的插件模板的完整目录在"RhinoBrush"文件夹中，其中也包含了上述文件，运行即可生成rhp插件文件。

插件运行，仍需下载rhinoSDK，下载链接：

https://www.rhino3d.com/download/Rhino-SDK/7.0/latest/
