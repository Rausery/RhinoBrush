// cmdRhinoBrush.cpp : command file
//
#include <iostream>
#include <fstream>
#include "StdAfx.h"
#include "RhinoBrushPlugIn.h"
#include "RhinoBrushClass.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN RhinoBrush command
//

#pragma region RhinoBrush command

// Do NOT put the definition of class CCommandRhinoBrush in a header
// file. There is only ONE instance of a CCommandRhinoBrush class
// and that instance is the static theRhinoBrushCommand that appears
// immediately below the class definition.

class CCommandRhinoBrush : public CRhinoCommand
{
public:
  // The one and only instance of CCommandRhinoBrush is created below.
  // No copy constructor or operator= is required.
  // Values of member variables persist for the duration of the application.

  // CCommandRhinoBrush::CCommandRhinoBrush()
  // is called exactly once when static theRhinoBrushCommand is created.
  CCommandRhinoBrush() = default;

  // CCommandRhinoBrush::~CCommandRhinoBrush()
  // is called exactly once when static theRhinoBrushCommand is destroyed.
  // The destructor should not make any calls to the Rhino SDK. 
  // If your command has persistent settings, then override 
  // CRhinoCommand::SaveProfile and CRhinoCommand::LoadProfile.
  ~CCommandRhinoBrush() = default;

  // Returns a unique UUID for this command.
  // If you try to use an id that is already being used, then
  // your command will not work. Use GUIDGEN.EXE to make unique UUID.
  UUID CommandUUID() override
  {
    // {53989CD6-7375-4F3C-A095-A7C58C444354}
    static const GUID RhinoBrushCommand_UUID =
    { 0x53989CD6, 0x7375, 0x4F3C, { 0xA0, 0x95, 0xA7, 0xC5, 0x8C, 0x44, 0x43, 0x54 } };
    return RhinoBrushCommand_UUID;
  }

  // Returns the English command name.
  // If you want to provide a localized command name, then override 
  // CRhinoCommand::LocalCommandName.
  const wchar_t* EnglishCommandName() override { return L"RhinoBrush"; }

  // Rhino calls RunCommand to run the command.
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandRhinoBrush object
// Do NOT create any other instance of a CCommandRhinoBrush class.
static class CCommandRhinoBrush theRhinoBrushCommand;

/////////////////////////////////执行函数体///////////////////////////////////////

CRhinoCommand::result CCommandRhinoBrush::RunCommand(const CRhinoCommandContext& context)
{

    //文档写入，执行信息
    std::fstream file;
    file.open("result.txt", std::ios_base::out);
    file.seekp(5);
    file << "start" << std::endl;
    //file.close();

    // Choose your edit method from Inflate/ScaleZ/Flatten
    // 选择编辑模式（0增减/1缩放/2压平）
    int Operation_Index = 0;

    CRhinoCommandOptionValue Operations[3];
    Operations[0] = RHCMDOPTVALUE(L"增减_Inflate");
    Operations[1] = RHCMDOPTVALUE(L"缩放_ScaleZ");
    Operations[2] = RHCMDOPTVALUE(L"压平_Flatten");

    // Choose she shape of the brush from Circle/Polygon/Rectangle/Customized
    // 选择笔刷形状（0圆形/1正多边形/2矩形/3自定义曲线）
    int Shape_Index = 0;

    CRhinoCommandOptionValue Shapes[4];
    Shapes[0] = RHCMDOPTVALUE(L"圆形_Circle");
    Shapes[1] = RHCMDOPTVALUE(L"正多边形_Polygon");
    Shapes[2] = RHCMDOPTVALUE(L"矩形_Rectangle");
    Shapes[3] = RHCMDOPTVALUE(L"自定义图形_Customized");

    CRhinoGetOption go;
    go.SetCommandPrompt(L"选择笔刷形状和操作类型");
    go.AcceptNothing();

    // 获取Operation_Index和Shape_Index
    for (;;)
    {
        // 生成相应的输入接口（套用的Rhino模板）
        go.ClearCommandOptions();
        int Operations_option_index = go.AddCommandOptionList(
            RHCMDOPTNAME(L"Operation"), 3, Operations, Operation_Index
        );
        int Shapes_option_index = go.AddCommandOptionList(
            RHCMDOPTNAME(L"Shape"), 4, Shapes, Shape_Index
        );

        // 流程控制（套用的Rhino模板）
        CRhinoGet::result res = go.GetOption();
        if (res == CRhinoGet::nothing) break;
        if (res == CRhinoGet::cancel) return CRhinoCommand::cancel;
        if (res != CRhinoGet::option) return CRhinoCommand::failure;
        const CRhinoCommandOption* option = go.Option();
        if (nullptr == option) return CRhinoCommand::failure;

        int option_index = option->m_option_index;
        // Set Operation_Index 选定编辑模式
        if (option_index == Operations_option_index) { Operation_Index = option->m_list_option_current; }
        // Set shape of the brush 设定笔刷形状
        if (option_index == Shapes_option_index) { Shape_Index = option->m_list_option_current; }

    }

    RhinoBrush* pBrush = nullptr;
    // 根据Operation_Index编辑模式（0增减/1缩放/2压平）
    // 和Shape_Index笔刷形状（0圆形/1正多边形/2矩形/3自定义曲线）
    // 生成相应的笔刷对象并储存参数

    switch (Operation_Index)
    {
        // 0 Inflate 增减
    case 0:
        switch (Shape_Index)
        {
            // 00 InflateCircle 圆形增减
        case 0: {
            // 需要用户输入的值
            double offset = 1;
            double strength = 1;
            double radius = 1;

            // 生成命令栏
            CRhinoGetOption go;
            go.SetCommandPrompt(L"设置InflateCircle笔刷参数");
            go.AcceptNothing();

            for (;;)
            {
                // 生成相应的输入接口
                go.ClearCommandOptions();
                int offset_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"offset"), &offset, L"double value", FALSE, -999.9, 999.9);
                int strength_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"strength"), &strength, L"double value", FALSE, -999.9, 999.9);
                int radius_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"radius"), &radius, L"double value", FALSE, -999.9, 999.9);

                // 流程控制
                CRhinoGet::result res = go.GetOption();
                if (res == CRhinoGet::nothing) break;
                if (res == CRhinoGet::cancel) return CRhinoCommand::cancel;
                if (res != CRhinoGet::option) return CRhinoCommand::failure;
                const CRhinoCommandOption* option = go.Option();
                if (nullptr == option) return CRhinoCommand::failure;

                // 用户输入
                int option_index = option->m_option_index;
                if (option_index == offset_option_index) continue;
                if (option_index == strength_option_index) continue;
                if (option_index == radius_option_index) continue;

            }

            // 生成笔刷实例
            InflateCircle Brush(offset, strength, radius);
            pBrush = &Brush;
        }
              break;

              // 01 InflatePolygon 正多边形增减
        case 1: {
            // 需要用户输入的值
            double offset = 1;
            int polynum = 3;
            double strength = 1;
            double radius = 1;

            // 生成命令栏
            CRhinoGetOption go;
            go.SetCommandPrompt(L"设置InflatePolygon笔刷参数");
            go.AcceptNothing();

            for (;;)
            {
                // 生成相应的输入接口
                go.ClearCommandOptions();
                int offset_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"offset"), &offset, L"double value", FALSE, -999.9, 999.9);
                int strength_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"strength"), &strength, L"double value", FALSE, -999.9, 999.9);
                int radius_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"radius"), &radius, L"double value", FALSE, -999.9, 999.9);
                int polynum_option_index = go.AddCommandOptionInteger(RHCMDOPTNAME(L"polynum"), &polynum, L"integer value", 1, 999);

                // 流程控制
                CRhinoGet::result res = go.GetOption();
                if (res == CRhinoGet::nothing) break;
                if (res == CRhinoGet::cancel) return CRhinoCommand::cancel;
                if (res != CRhinoGet::option) return CRhinoCommand::failure;
                const CRhinoCommandOption* option = go.Option();
                if (nullptr == option) return CRhinoCommand::failure;

                // 用户输入
                int option_index = option->m_option_index;
                if (option_index == offset_option_index) continue;
                if (option_index == strength_option_index) continue;
                if (option_index == radius_option_index) continue;
                if (option_index == polynum_option_index) continue;

            }

            // 生成笔刷实例
            InflatePolygon Brush(offset, strength, radius, polynum);
            pBrush = &Brush;
        }
              break;

              // 02 InflateRectangle 矩形增减
        case 2: {
            // 需要用户输入的值
            double offset = 1;
            double strength = 1;
            double width = 1;
            double length = 1;

            // 生成命令栏
            CRhinoGetOption go;
            go.SetCommandPrompt(L"设置InflateRectangle笔刷参数");
            go.AcceptNothing();

            for (;;)
            {
                // 生成相应的输入接口
                go.ClearCommandOptions();
                int offset_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"offset"), &offset, L"double value", FALSE, -999.9, 999.9);
                int strength_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"strength"), &strength, L"double value", FALSE, -999.9, 999.9);
                int width_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"width"), &width, L"double value", FALSE, -999.9, 999.9);
                int length_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"length"), &length, L"double value", FALSE, -999.9, 999.9);

                // 流程控制
                CRhinoGet::result res = go.GetOption();
                if (res == CRhinoGet::nothing) break;
                if (res == CRhinoGet::cancel) return CRhinoCommand::cancel;
                if (res != CRhinoGet::option) return CRhinoCommand::failure;
                const CRhinoCommandOption* option = go.Option();
                if (nullptr == option) return CRhinoCommand::failure;

                // 用户输入
                int option_index = option->m_option_index;
                if (option_index == offset_option_index) continue;
                if (option_index == strength_option_index) continue;
                if (option_index == width_option_index) continue;
                if (option_index == length_option_index) continue;

            }

            // 生成笔刷实例
            InflateRectangle Brush(offset, strength, width, length);
            pBrush = &Brush;
        }
              break;

              // 03 InflateCustomized 自定义曲线增减
        case 3: {
            // 需要用户输入的值
            double offset = 1;
            double strength = 1;

            // 生成命令栏
            CRhinoGetOption go;
            go.SetCommandPrompt(L"设置InflateCustomized笔刷参数");
            go.AcceptNothing();

            for (;;)
            {
                // 生成相应的输入接口
                go.ClearCommandOptions();
                int offset_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"offset"), &offset, L"double value", FALSE, -999.9, 999.9);
                int strength_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"strength"), &strength, L"double value", FALSE, -999.9, 999.9);

                // 流程控制
                CRhinoGet::result res = go.GetOption();
                if (res == CRhinoGet::nothing) break;
                if (res == CRhinoGet::cancel) return CRhinoCommand::cancel;
                if (res != CRhinoGet::option) return CRhinoCommand::failure;
                const CRhinoCommandOption* option = go.Option();
                if (nullptr == option) return CRhinoCommand::failure;

                // 用户输入
                int option_index = option->m_option_index;
                if (option_index == offset_option_index) continue;
                if (option_index == strength_option_index) continue;

            }

            //获取自定义曲线
            CRhinoGetObject get;
            get.SetCommandPrompt(L"选择自定义曲线");
            get.SetGeometryFilter(CRhinoGetObject::curve_object);
            CRhinoGet::result res = get.GetObjects(1, 1);
            ON_NurbsCurve* customizedcurve = nullptr;
            if (res == CRhinoGet::object)
            {
                const CRhinoObjRef& objref = get.Object(0);
                const ON_Curve* curve = objref.Curve();
                if (curve)
                {
                    if (curve->IsClosed()) {
                        customizedcurve = curve->NurbsCurve();
                        // 生成笔刷实例
                        InflateCustomized Brush(offset, strength, customizedcurve);
                        pBrush = &Brush;
                        break;
                    }
                    else
                    {
                        RhinoApp().Print(L"请选择一个封闭的曲线");
                        return CRhinoCommand::success;
                    }
                }
            }
            else
            {
                RhinoApp().Print(L"请选择一个曲线");
                return CRhinoCommand::success;
            }
        }
              break;
        }
        break;

        // 1 ScaleZ 伸缩
    case 1:
        switch (Shape_Index)
        {
            // 10 ScaleZCircle 圆形伸缩
        case 0: {
            // 需要用户输入的值
            double offset = 1;
            double strength = 1;
            double radius = 1;

            // 生成命令栏
            CRhinoGetOption go;
            go.SetCommandPrompt(L"设置ScaleZCircle笔刷参数");
            go.AcceptNothing();

            for (;;)
            {
                // 生成相应的输入接口
                go.ClearCommandOptions();
                int offset_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"offset"), &offset, L"double value", FALSE, -999.9, 999.9);
                int strength_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"scale"), &strength, L"double value", FALSE, -999.9, 999.9);
                int radius_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"radius"), &radius, L"double value", FALSE, -999.9, 999.9);

                // 流程控制
                CRhinoGet::result res = go.GetOption();
                if (res == CRhinoGet::nothing) break;
                if (res == CRhinoGet::cancel) return CRhinoCommand::cancel;
                if (res != CRhinoGet::option) return CRhinoCommand::failure;
                const CRhinoCommandOption* option = go.Option();
                if (nullptr == option) return CRhinoCommand::failure;

                // 用户输入
                int option_index = option->m_option_index;
                if (option_index == offset_option_index) continue;
                if (option_index == strength_option_index) continue;
                if (option_index == radius_option_index) continue;

            }

            // 生成笔刷实例
            ScaleZCircle Brush(offset, strength, radius);
            pBrush = &Brush;
        }
              break;

              // 11 ScaleZPolygon 正多边形伸缩
        case 1: {
            // 需要用户输入的值
            double offset = 1;
            int polynum = 3;
            double strength = 1;
            double radius = 1;

            // 生成命令栏
            CRhinoGetOption go;
            go.SetCommandPrompt(L"设置ScaleZPolygon笔刷参数");
            go.AcceptNothing();

            for (;;)
            {
                // 生成相应的输入接口
                go.ClearCommandOptions();
                int offset_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"offset"), &offset, L"double value", FALSE, -999.9, 999.9);
                int strength_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"scale"), &strength, L"double value", FALSE, -999.9, 999.9);
                int radius_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"radius"), &radius, L"double value", FALSE, -999.9, 999.9);
                int polynum_option_index = go.AddCommandOptionInteger(RHCMDOPTNAME(L"polynum"), &polynum, L"integer value", 1, 999);

                // 流程控制
                CRhinoGet::result res = go.GetOption();
                if (res == CRhinoGet::nothing) break;
                if (res == CRhinoGet::cancel) return CRhinoCommand::cancel;
                if (res != CRhinoGet::option) return CRhinoCommand::failure;
                const CRhinoCommandOption* option = go.Option();
                if (nullptr == option) return CRhinoCommand::failure;

                // 用户输入
                int option_index = option->m_option_index;
                if (option_index == offset_option_index) continue;
                if (option_index == strength_option_index) continue;
                if (option_index == radius_option_index) continue;
                if (option_index == polynum_option_index) continue;

            }

            // 生成笔刷实例
            ScaleZPolygon Brush(offset, strength, radius, polynum);
            pBrush = &Brush;
        }
              break;

              // 12 ScaleZRectangle 矩形伸缩
        case 2: {
            // 需要用户输入的值
            double offset = 1;
            double strength = 1;
            double width = 1;
            double length = 1;

            // 生成命令栏
            CRhinoGetOption go;
            go.SetCommandPrompt(L"设置ScaleZRectangle笔刷参数");
            go.AcceptNothing();

            for (;;)
            {
                // 生成相应的输入接口
                go.ClearCommandOptions();
                int offset_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"offset"), &offset, L"double value", FALSE, -999.9, 999.9);
                int strength_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"scale"), &strength, L"double value", FALSE, -999.9, 999.9);
                int width_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"width"), &width, L"double value", FALSE, -999.9, 999.9);
                int length_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"length"), &length, L"double value", FALSE, -999.9, 999.9);

                // 流程控制
                CRhinoGet::result res = go.GetOption();
                if (res == CRhinoGet::nothing) break;
                if (res == CRhinoGet::cancel) return CRhinoCommand::cancel;
                if (res != CRhinoGet::option) return CRhinoCommand::failure;
                const CRhinoCommandOption* option = go.Option();
                if (nullptr == option) return CRhinoCommand::failure;

                // 用户输入
                int option_index = option->m_option_index;
                if (option_index == offset_option_index) continue;
                if (option_index == strength_option_index) continue;
                if (option_index == width_option_index) continue;
                if (option_index == length_option_index) continue;

            }

            // 生成笔刷实例
            ScaleZRectangle Brush(offset, strength, width, length);
            pBrush = &Brush;
        }
              break;

              // 13 ScaleZCustomized 自定义曲线伸缩
        case 3: {
            // 需要用户输入的值
            double offset = 1;
            double strength = 1;

            // 生成命令栏
            CRhinoGetOption go;
            go.SetCommandPrompt(L"设置ScaleZCustomized笔刷参数");
            go.AcceptNothing();

            for (;;)
            {
                // 生成相应的输入接口
                go.ClearCommandOptions();
                int offset_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"offset"), &offset, L"double value", FALSE, -999.9, 999.9);
                int strength_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"scale"), &strength, L"double value", FALSE, -999.9, 999.9);

                // 流程控制
                CRhinoGet::result res = go.GetOption();
                if (res == CRhinoGet::nothing) break;
                if (res == CRhinoGet::cancel) return CRhinoCommand::cancel;
                if (res != CRhinoGet::option) return CRhinoCommand::failure;
                const CRhinoCommandOption* option = go.Option();
                if (nullptr == option) return CRhinoCommand::failure;

                // 用户输入
                int option_index = option->m_option_index;
                if (option_index == offset_option_index) continue;
                if (option_index == strength_option_index) continue;

            }

            //获取自定义曲线
            CRhinoGetObject get;
            get.SetCommandPrompt(L"选择自定义曲线");
            get.SetGeometryFilter(CRhinoGetObject::curve_object);
            CRhinoGet::result res = get.GetObjects(1, 1);
            ON_NurbsCurve* customizedcurve = nullptr;
            if (res == CRhinoGet::object)
            {
                const CRhinoObjRef& objref = get.Object(0);
                const ON_Curve* curve = objref.Curve();
                if (curve)
                {
                    if (curve->IsClosed()) {
                        customizedcurve = curve->NurbsCurve();
                        // 生成笔刷实例
                        ScaleZCustomized Brush(offset, strength, customizedcurve);
                        pBrush = &Brush;
                        break;
                    }
                    else
                    {
                        RhinoApp().Print(L"请选择一个封闭的曲线");
                        return CRhinoCommand::success;
                    }
                }
                else
                {
                    RhinoApp().Print(L"请选择一个曲线");
                    return CRhinoCommand::success;
                }
            }
        }
              break;
        }
        break;

        // 2 Flatten 压平
    case 2:
        switch (Shape_Index)
        {
            // 20 FlattenCircle 圆形压平
        case 0: {
            // 需要用户输入的值
            double offset = 1;
            double targetheight = 1;
            double radius = 1;

            // 生成命令栏
            CRhinoGetOption go;
            go.SetCommandPrompt(L"设置FlattenCircle笔刷参数");
            go.AcceptNothing();

            for (;;)
            {
                // 生成相应的输入接口
                go.ClearCommandOptions();
                int offset_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"offset"), &offset, L"double value", FALSE, -999.9, 999.9);
                int targetheight_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"targetheight"), &targetheight, L"double value", FALSE, -999.9, 999.9);
                int radius_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"radius"), &radius, L"double value", FALSE, -999.9, 999.9);

                // 流程控制
                CRhinoGet::result res = go.GetOption();
                if (res == CRhinoGet::nothing) break;
                if (res == CRhinoGet::cancel) return CRhinoCommand::cancel;
                if (res != CRhinoGet::option) return CRhinoCommand::failure;
                const CRhinoCommandOption* option = go.Option();
                if (nullptr == option) return CRhinoCommand::failure;

                // 用户输入
                int option_index = option->m_option_index;
                if (option_index == offset_option_index) continue;
                if (option_index == targetheight_option_index) continue;
                if (option_index == radius_option_index) continue;

            }

            // 生成笔刷实例
            FlattenCircle Brush(offset, targetheight, radius);
            pBrush = &Brush;
            break;
        }

              // 21 FlattenPolygon 正多边形压平
        case 1: {
            // 需要用户输入的值
            double offset = 1;
            int polynum = 3;
            double targetheight = 1;
            double radius = 1;

            // 生成命令栏
            CRhinoGetOption go;
            go.SetCommandPrompt(L"设置FlattenPolygon笔刷参数");
            go.AcceptNothing();

            for (;;)
            {
                // 生成相应的输入接口
                go.ClearCommandOptions();
                int offset_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"offset"), &offset, L"double value", FALSE, -999.9, 999.9);
                int targetheight_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"targetheight"), &targetheight, L"double value", FALSE, -999.9, 999.9);
                int radius_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"radius"), &radius, L"double value", FALSE, -999.9, 999.9);
                int polynum_option_index = go.AddCommandOptionInteger(RHCMDOPTNAME(L"polynum"), &polynum, L"integer value", 1, 999);

                // 流程控制
                CRhinoGet::result res = go.GetOption();
                if (res == CRhinoGet::nothing) break;
                if (res == CRhinoGet::cancel) return CRhinoCommand::cancel;
                if (res != CRhinoGet::option) return CRhinoCommand::failure;
                const CRhinoCommandOption* option = go.Option();
                if (nullptr == option) return CRhinoCommand::failure;

                // 用户输入
                int option_index = option->m_option_index;
                if (option_index == offset_option_index) continue;
                if (option_index == targetheight_option_index) continue;
                if (option_index == radius_option_index) continue;
                if (option_index == polynum_option_index) continue;

            }

            // 生成笔刷实例
            FlattenPolygon Brush(offset, targetheight, radius, polynum);
            pBrush = &Brush;
        }
              break;

              // 22 FlattenRectangle 矩形压平
        case 2: {
            // 需要用户输入的值
            double offset = 1;
            double targetheight = 1;
            double width = 1;
            double length = 1;

            // 生成命令栏
            CRhinoGetOption go;
            go.SetCommandPrompt(L"设置FlattenRectangle笔刷参数");
            go.AcceptNothing();

            for (;;)
            {
                // 生成相应的输入接口
                go.ClearCommandOptions();
                int offset_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"offset"), &offset, L"double value", FALSE, -999.9, 999.9);
                int targetheight_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"targetheight"), &targetheight, L"double value", FALSE, -999.9, 999.9);
                int width_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"width"), &width, L"double value", FALSE, -999.9, 999.9);
                int length_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"length"), &length, L"double value", FALSE, -999.9, 999.9);

                // 流程控制
                CRhinoGet::result res = go.GetOption();
                if (res == CRhinoGet::nothing) break;
                if (res == CRhinoGet::cancel) return CRhinoCommand::cancel;
                if (res != CRhinoGet::option) return CRhinoCommand::failure;
                const CRhinoCommandOption* option = go.Option();
                if (nullptr == option) return CRhinoCommand::failure;

                // 用户输入
                int option_index = option->m_option_index;
                if (option_index == offset_option_index) continue;
                if (option_index == targetheight_option_index) continue;
                if (option_index == width_option_index) continue;
                if (option_index == length_option_index) continue;

            }

            // 生成笔刷实例
            FlattenRectangle Brush(offset, targetheight, width, length);
            pBrush = &Brush;
        }
              break;

              // 23 FlattenCustomized 自定义曲线压平
        case 3: {
            // 需要用户输入的值
            double offset = 1;
            double targetheight = 1;

            // 生成命令栏
            CRhinoGetOption go;
            go.SetCommandPrompt(L"设置FlattenCustomized笔刷参数");
            go.AcceptNothing();

            for (;;)
            {
                // 生成相应的输入接口
                go.ClearCommandOptions();
                int offset_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"offset"), &offset, L"double value", FALSE, -999.9, 999.9);
                int targetheight_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"targetheight"), &targetheight, L"double value", FALSE, -999.9, 999.9);

                // 流程控制
                CRhinoGet::result res = go.GetOption();
                if (res == CRhinoGet::nothing) break;
                if (res == CRhinoGet::cancel) return CRhinoCommand::cancel;
                if (res != CRhinoGet::option) return CRhinoCommand::failure;
                const CRhinoCommandOption* option = go.Option();
                if (nullptr == option) return CRhinoCommand::failure;

                // 用户输入
                int option_index = option->m_option_index;
                if (option_index == offset_option_index) continue;
                if (option_index == targetheight_option_index) continue;

            }

            //获取自定义曲线
            CRhinoGetObject get;
            get.SetCommandPrompt(L"选择自定义曲线");
            get.SetGeometryFilter(CRhinoGetObject::curve_object);
            CRhinoGet::result res = get.GetObjects(1, 1);
            ON_NurbsCurve* customizedcurve = nullptr;
            if (res == CRhinoGet::object)
            {
                const CRhinoObjRef& objref = get.Object(0);
                const ON_Curve* curve = objref.Curve();
                if (curve)
                {
                    if (curve->IsClosed()) {
                        customizedcurve = curve->NurbsCurve();
                        // 生成笔刷实例
                        FlattenCustomized Brush(offset, targetheight, customizedcurve);
                        pBrush = &Brush;
                        break;
                    }
                    else
                    {
                        RhinoApp().Print(L"请选择一个封闭的曲线");
                        return CRhinoCommand::success;
                    }
                }
                else
                {
                    RhinoApp().Print(L"请选择一个曲线");
                    return CRhinoCommand::success;
                }
            }
        }
              break;
        }
        break;
    }

    ////////////// 定义画笔之后的执行

    // 获取编辑对象曲面
    CRhinoGetObject getsurface;
    getsurface.SetCommandPrompt(L"Select a surface");
    getsurface.SetGeometryFilter(CRhinoGetObject::surface_object);
    CRhinoGet::result res = getsurface.GetObjects(1, 1);

    if (res != CRhinoGet::object)
    {
        return CRhinoCommand::failure;
    }
    const CRhinoObjRef& surface_ref = getsurface.Object(0);
    const ON_Surface* srf = surface_ref.Surface();

    ON_NurbsSurface* nurbs_srf = srf->NurbsSurface();

    // GetPtrCV
    //取surface 的UV值
    const int srf_U = nurbs_srf->CVCount(0);
    const int srf_V = nurbs_srf->CVCount(1);
    const int allCVcount = srf_U * srf_V;

    // 控制点数组
    ON_3dPoint* ptrCV = new ON_3dPoint[allCVcount];

    // 待选取控制点数组
    CRhinoObjRef* ptref = new CRhinoObjRef[allCVcount];

    // 中间形态曲面
    CRhinoObjRef last_nurbs_surface;

    // 重复执行编辑命令
    for (;;) {

        // 开vector数组保存在内圈内的点和外圈内的点在CV中的位置
        std::vector<int> indices_in;
        std::vector<int> indices_out;

        // 清除先前的控制点
        for (int i = 0; i < srf_U; i++)
        {
            for (int j = 0; j < srf_V; j++)
            {
                int index = i * srf_U + j;
                context.m_doc.DeleteObject(ptref[index]);
            }
        }

        // 显示所有的控制点
        for (int i = 0; i < srf_U; i++)
        {
            for (int j = 0; j < srf_V; j++)
            {
                int index = i * srf_U + j;
                ON_3dPoint now = ptrCV[index];
                nurbs_srf->GetCV(i, j, now);
                CRhinoPointObject* pt = context.m_doc.AddPointObject(now);
                ptref[index] = pt;
            }
        }
        context.m_doc.Redraw();

        // 获取编辑对象点
        CRhinoGetObject getpoint;
        getpoint.SetCommandPrompt(L"Select a point");
        getpoint.SetGeometryFilter(CRhinoGetObject::point_object);
        CRhinoGet::result res = getpoint.GetObjects(1, 1);

        if (res != CRhinoGet::object) break;

        const CRhinoObjRef& obj_ref = getpoint.Object(0);
        const ON_Point* pt = obj_ref.Point();

        ON_3dPoint editpoint(*pt);
        getpoint.ClearCommandOptions();

        

        // 获取内外曲线
        ON_NurbsCurve innercurve;
        ON_NurbsCurve outercurve;
        
        if (pBrush->pShape->GetInnerAndOuterCurve(&innercurve, &outercurve, editpoint) != S_OK)
        {
            file << "GetInnerAndOuterCurve function ERROR" << std::endl;
            return CRhinoCommand::failure;
        }

        // 展示内外曲线
        //CRhinoCurveObject* in_object = context.m_doc.AddCurveObject(innercurve);
        //CRhinoCurveObject* out_object = context.m_doc.AddCurveObject(outercurve);
        //context.m_doc.Redraw();

        for (int i = 0; i < srf_U; i++)
        {
            for (int j = 0; j < srf_V; j++)
            {
                int index = i * srf_U + j;
                ON_3dPoint now = ptrCV[index];
                nurbs_srf->GetCV(i, j, now);
                ptrCV[index].x = now.x;
                ptrCV[index].y = now.y;
                ptrCV[index].z = now.z;

                bool inner = 0;
                pBrush->pShape->IsPointIn(ptrCV[index], &innercurve, inner);
                bool outer = 0;
                pBrush->pShape->IsPointIn(ptrCV[index], &outercurve, outer);
                if (inner)
                {
                    indices_in.push_back(index);
                }

                else if (outer && !inner)
                {
                    indices_out.push_back(index);
                }
            }
        }
        //现在拿到了所有在内圈范围内以及所有在内外圈之间的的点的index；

        int inCount = indices_in.size();
        int outCount = indices_out.size();

        //改为常规数组
        int* indices_IN = new int[inCount];
        int* indices_OUT = new int[outCount];

        for (int i = 0; i < inCount; i++)
        {
            indices_IN[i] = indices_in[i];
        }
        for (int i = 0; i < outCount; i++)
        {
            indices_OUT[i] = indices_out[i];
        }

        //【1、用修改完的控制点重新生成一个新的曲面】
        //【2、单步修改原曲面的每一个控制点】
        // 目前用第2种。

        //【for inner points】
        if (pBrush->pOperation->InnerEdit(nurbs_srf, ptrCV, indices_IN, inCount) != S_OK)
        {
            file << "InnerEdit function ERROR" <<std:: endl;
            return CRhinoCommand::failure;
        }
        

        //【for outer points】
        if (OuterEdit(nurbs_srf, &innercurve, &outercurve, ptrCV, indices_OUT, indices_IN, outCount, inCount)!= S_OK)
        {
            file << "OuterEdit function ERROR"<<std::endl;
            return CRhinoCommand::failure;
        }

        delete[] indices_IN;
        delete[] indices_OUT;

        // 删除初始surface
        context.m_doc.DeleteObject(surface_ref);

        // 删除上一个surface
        context.m_doc.DeleteObject(last_nurbs_surface);

        // 用当前的surface初始化CRhinoObjRef new_nurbs_surface
        CRhinoObjRef new_nurbs_surface(context.m_doc.AddSurfaceObject(*nurbs_srf));

        last_nurbs_surface = new_nurbs_surface;
        context.m_doc.Redraw();
        RhinoApp().ActiveDoc()->Redraw();

    }

    RhinoApp().Print(L"START");
    for (int i = 0; i < srf_U; i++)
    {
        for (int j = 0; j < srf_V; j++)
        {
            int index = i * srf_U + j;
            context.m_doc.DeleteObject(ptref[index]);
        }
    }
    RhinoApp().Print(L"END");
    delete[] ptrCV;

    file << "SUCCESS!" <<std::endl;
    file.close();
    return CRhinoCommand::success;
}

#pragma endregion

//
// END PlugIn1 command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
