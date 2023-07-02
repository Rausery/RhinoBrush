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

/////////////////////////////////ִ�к�����///////////////////////////////////////

CRhinoCommand::result CCommandRhinoBrush::RunCommand(const CRhinoCommandContext& context)
{

    //�ĵ�д�룬ִ����Ϣ
    std::fstream file;
    file.open("result.txt", std::ios_base::out);
    file.seekp(5);
    file << "start" << std::endl;
    //file.close();

    // Choose your edit method from Inflate/ScaleZ/Flatten
    // ѡ��༭ģʽ��0����/1����/2ѹƽ��
    int Operation_Index = 0;

    CRhinoCommandOptionValue Operations[3];
    Operations[0] = RHCMDOPTVALUE(L"����_Inflate");
    Operations[1] = RHCMDOPTVALUE(L"����_ScaleZ");
    Operations[2] = RHCMDOPTVALUE(L"ѹƽ_Flatten");

    // Choose she shape of the brush from Circle/Polygon/Rectangle/Customized
    // ѡ���ˢ��״��0Բ��/1�������/2����/3�Զ������ߣ�
    int Shape_Index = 0;

    CRhinoCommandOptionValue Shapes[4];
    Shapes[0] = RHCMDOPTVALUE(L"Բ��_Circle");
    Shapes[1] = RHCMDOPTVALUE(L"�������_Polygon");
    Shapes[2] = RHCMDOPTVALUE(L"����_Rectangle");
    Shapes[3] = RHCMDOPTVALUE(L"�Զ���ͼ��_Customized");

    CRhinoGetOption go;
    go.SetCommandPrompt(L"ѡ���ˢ��״�Ͳ�������");
    go.AcceptNothing();

    // ��ȡOperation_Index��Shape_Index
    for (;;)
    {
        // ������Ӧ������ӿڣ����õ�Rhinoģ�壩
        go.ClearCommandOptions();
        int Operations_option_index = go.AddCommandOptionList(
            RHCMDOPTNAME(L"Operation"), 3, Operations, Operation_Index
        );
        int Shapes_option_index = go.AddCommandOptionList(
            RHCMDOPTNAME(L"Shape"), 4, Shapes, Shape_Index
        );

        // ���̿��ƣ����õ�Rhinoģ�壩
        CRhinoGet::result res = go.GetOption();
        if (res == CRhinoGet::nothing) break;
        if (res == CRhinoGet::cancel) return CRhinoCommand::cancel;
        if (res != CRhinoGet::option) return CRhinoCommand::failure;
        const CRhinoCommandOption* option = go.Option();
        if (nullptr == option) return CRhinoCommand::failure;

        int option_index = option->m_option_index;
        // Set Operation_Index ѡ���༭ģʽ
        if (option_index == Operations_option_index) { Operation_Index = option->m_list_option_current; }
        // Set shape of the brush �趨��ˢ��״
        if (option_index == Shapes_option_index) { Shape_Index = option->m_list_option_current; }

    }

    RhinoBrush* pBrush = nullptr;
    // ����Operation_Index�༭ģʽ��0����/1����/2ѹƽ��
    // ��Shape_Index��ˢ��״��0Բ��/1�������/2����/3�Զ������ߣ�
    // ������Ӧ�ı�ˢ���󲢴������

    switch (Operation_Index)
    {
        // 0 Inflate ����
    case 0:
        switch (Shape_Index)
        {
            // 00 InflateCircle Բ������
        case 0: {
            // ��Ҫ�û������ֵ
            double offset = 1;
            double strength = 1;
            double radius = 1;

            // ����������
            CRhinoGetOption go;
            go.SetCommandPrompt(L"����InflateCircle��ˢ����");
            go.AcceptNothing();

            for (;;)
            {
                // ������Ӧ������ӿ�
                go.ClearCommandOptions();
                int offset_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"offset"), &offset, L"double value", FALSE, -999.9, 999.9);
                int strength_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"strength"), &strength, L"double value", FALSE, -999.9, 999.9);
                int radius_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"radius"), &radius, L"double value", FALSE, -999.9, 999.9);

                // ���̿���
                CRhinoGet::result res = go.GetOption();
                if (res == CRhinoGet::nothing) break;
                if (res == CRhinoGet::cancel) return CRhinoCommand::cancel;
                if (res != CRhinoGet::option) return CRhinoCommand::failure;
                const CRhinoCommandOption* option = go.Option();
                if (nullptr == option) return CRhinoCommand::failure;

                // �û�����
                int option_index = option->m_option_index;
                if (option_index == offset_option_index) continue;
                if (option_index == strength_option_index) continue;
                if (option_index == radius_option_index) continue;

            }

            // ���ɱ�ˢʵ��
            InflateCircle Brush(offset, strength, radius);
            pBrush = &Brush;
        }
              break;

              // 01 InflatePolygon �����������
        case 1: {
            // ��Ҫ�û������ֵ
            double offset = 1;
            int polynum = 3;
            double strength = 1;
            double radius = 1;

            // ����������
            CRhinoGetOption go;
            go.SetCommandPrompt(L"����InflatePolygon��ˢ����");
            go.AcceptNothing();

            for (;;)
            {
                // ������Ӧ������ӿ�
                go.ClearCommandOptions();
                int offset_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"offset"), &offset, L"double value", FALSE, -999.9, 999.9);
                int strength_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"strength"), &strength, L"double value", FALSE, -999.9, 999.9);
                int radius_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"radius"), &radius, L"double value", FALSE, -999.9, 999.9);
                int polynum_option_index = go.AddCommandOptionInteger(RHCMDOPTNAME(L"polynum"), &polynum, L"integer value", 1, 999);

                // ���̿���
                CRhinoGet::result res = go.GetOption();
                if (res == CRhinoGet::nothing) break;
                if (res == CRhinoGet::cancel) return CRhinoCommand::cancel;
                if (res != CRhinoGet::option) return CRhinoCommand::failure;
                const CRhinoCommandOption* option = go.Option();
                if (nullptr == option) return CRhinoCommand::failure;

                // �û�����
                int option_index = option->m_option_index;
                if (option_index == offset_option_index) continue;
                if (option_index == strength_option_index) continue;
                if (option_index == radius_option_index) continue;
                if (option_index == polynum_option_index) continue;

            }

            // ���ɱ�ˢʵ��
            InflatePolygon Brush(offset, strength, radius, polynum);
            pBrush = &Brush;
        }
              break;

              // 02 InflateRectangle ��������
        case 2: {
            // ��Ҫ�û������ֵ
            double offset = 1;
            double strength = 1;
            double width = 1;
            double length = 1;

            // ����������
            CRhinoGetOption go;
            go.SetCommandPrompt(L"����InflateRectangle��ˢ����");
            go.AcceptNothing();

            for (;;)
            {
                // ������Ӧ������ӿ�
                go.ClearCommandOptions();
                int offset_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"offset"), &offset, L"double value", FALSE, -999.9, 999.9);
                int strength_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"strength"), &strength, L"double value", FALSE, -999.9, 999.9);
                int width_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"width"), &width, L"double value", FALSE, -999.9, 999.9);
                int length_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"length"), &length, L"double value", FALSE, -999.9, 999.9);

                // ���̿���
                CRhinoGet::result res = go.GetOption();
                if (res == CRhinoGet::nothing) break;
                if (res == CRhinoGet::cancel) return CRhinoCommand::cancel;
                if (res != CRhinoGet::option) return CRhinoCommand::failure;
                const CRhinoCommandOption* option = go.Option();
                if (nullptr == option) return CRhinoCommand::failure;

                // �û�����
                int option_index = option->m_option_index;
                if (option_index == offset_option_index) continue;
                if (option_index == strength_option_index) continue;
                if (option_index == width_option_index) continue;
                if (option_index == length_option_index) continue;

            }

            // ���ɱ�ˢʵ��
            InflateRectangle Brush(offset, strength, width, length);
            pBrush = &Brush;
        }
              break;

              // 03 InflateCustomized �Զ�����������
        case 3: {
            // ��Ҫ�û������ֵ
            double offset = 1;
            double strength = 1;

            // ����������
            CRhinoGetOption go;
            go.SetCommandPrompt(L"����InflateCustomized��ˢ����");
            go.AcceptNothing();

            for (;;)
            {
                // ������Ӧ������ӿ�
                go.ClearCommandOptions();
                int offset_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"offset"), &offset, L"double value", FALSE, -999.9, 999.9);
                int strength_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"strength"), &strength, L"double value", FALSE, -999.9, 999.9);

                // ���̿���
                CRhinoGet::result res = go.GetOption();
                if (res == CRhinoGet::nothing) break;
                if (res == CRhinoGet::cancel) return CRhinoCommand::cancel;
                if (res != CRhinoGet::option) return CRhinoCommand::failure;
                const CRhinoCommandOption* option = go.Option();
                if (nullptr == option) return CRhinoCommand::failure;

                // �û�����
                int option_index = option->m_option_index;
                if (option_index == offset_option_index) continue;
                if (option_index == strength_option_index) continue;

            }

            //��ȡ�Զ�������
            CRhinoGetObject get;
            get.SetCommandPrompt(L"ѡ���Զ�������");
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
                        // ���ɱ�ˢʵ��
                        InflateCustomized Brush(offset, strength, customizedcurve);
                        pBrush = &Brush;
                        break;
                    }
                    else
                    {
                        RhinoApp().Print(L"��ѡ��һ����յ�����");
                        return CRhinoCommand::success;
                    }
                }
            }
            else
            {
                RhinoApp().Print(L"��ѡ��һ������");
                return CRhinoCommand::success;
            }
        }
              break;
        }
        break;

        // 1 ScaleZ ����
    case 1:
        switch (Shape_Index)
        {
            // 10 ScaleZCircle Բ������
        case 0: {
            // ��Ҫ�û������ֵ
            double offset = 1;
            double strength = 1;
            double radius = 1;

            // ����������
            CRhinoGetOption go;
            go.SetCommandPrompt(L"����ScaleZCircle��ˢ����");
            go.AcceptNothing();

            for (;;)
            {
                // ������Ӧ������ӿ�
                go.ClearCommandOptions();
                int offset_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"offset"), &offset, L"double value", FALSE, -999.9, 999.9);
                int strength_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"scale"), &strength, L"double value", FALSE, -999.9, 999.9);
                int radius_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"radius"), &radius, L"double value", FALSE, -999.9, 999.9);

                // ���̿���
                CRhinoGet::result res = go.GetOption();
                if (res == CRhinoGet::nothing) break;
                if (res == CRhinoGet::cancel) return CRhinoCommand::cancel;
                if (res != CRhinoGet::option) return CRhinoCommand::failure;
                const CRhinoCommandOption* option = go.Option();
                if (nullptr == option) return CRhinoCommand::failure;

                // �û�����
                int option_index = option->m_option_index;
                if (option_index == offset_option_index) continue;
                if (option_index == strength_option_index) continue;
                if (option_index == radius_option_index) continue;

            }

            // ���ɱ�ˢʵ��
            ScaleZCircle Brush(offset, strength, radius);
            pBrush = &Brush;
        }
              break;

              // 11 ScaleZPolygon �����������
        case 1: {
            // ��Ҫ�û������ֵ
            double offset = 1;
            int polynum = 3;
            double strength = 1;
            double radius = 1;

            // ����������
            CRhinoGetOption go;
            go.SetCommandPrompt(L"����ScaleZPolygon��ˢ����");
            go.AcceptNothing();

            for (;;)
            {
                // ������Ӧ������ӿ�
                go.ClearCommandOptions();
                int offset_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"offset"), &offset, L"double value", FALSE, -999.9, 999.9);
                int strength_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"scale"), &strength, L"double value", FALSE, -999.9, 999.9);
                int radius_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"radius"), &radius, L"double value", FALSE, -999.9, 999.9);
                int polynum_option_index = go.AddCommandOptionInteger(RHCMDOPTNAME(L"polynum"), &polynum, L"integer value", 1, 999);

                // ���̿���
                CRhinoGet::result res = go.GetOption();
                if (res == CRhinoGet::nothing) break;
                if (res == CRhinoGet::cancel) return CRhinoCommand::cancel;
                if (res != CRhinoGet::option) return CRhinoCommand::failure;
                const CRhinoCommandOption* option = go.Option();
                if (nullptr == option) return CRhinoCommand::failure;

                // �û�����
                int option_index = option->m_option_index;
                if (option_index == offset_option_index) continue;
                if (option_index == strength_option_index) continue;
                if (option_index == radius_option_index) continue;
                if (option_index == polynum_option_index) continue;

            }

            // ���ɱ�ˢʵ��
            ScaleZPolygon Brush(offset, strength, radius, polynum);
            pBrush = &Brush;
        }
              break;

              // 12 ScaleZRectangle ��������
        case 2: {
            // ��Ҫ�û������ֵ
            double offset = 1;
            double strength = 1;
            double width = 1;
            double length = 1;

            // ����������
            CRhinoGetOption go;
            go.SetCommandPrompt(L"����ScaleZRectangle��ˢ����");
            go.AcceptNothing();

            for (;;)
            {
                // ������Ӧ������ӿ�
                go.ClearCommandOptions();
                int offset_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"offset"), &offset, L"double value", FALSE, -999.9, 999.9);
                int strength_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"scale"), &strength, L"double value", FALSE, -999.9, 999.9);
                int width_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"width"), &width, L"double value", FALSE, -999.9, 999.9);
                int length_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"length"), &length, L"double value", FALSE, -999.9, 999.9);

                // ���̿���
                CRhinoGet::result res = go.GetOption();
                if (res == CRhinoGet::nothing) break;
                if (res == CRhinoGet::cancel) return CRhinoCommand::cancel;
                if (res != CRhinoGet::option) return CRhinoCommand::failure;
                const CRhinoCommandOption* option = go.Option();
                if (nullptr == option) return CRhinoCommand::failure;

                // �û�����
                int option_index = option->m_option_index;
                if (option_index == offset_option_index) continue;
                if (option_index == strength_option_index) continue;
                if (option_index == width_option_index) continue;
                if (option_index == length_option_index) continue;

            }

            // ���ɱ�ˢʵ��
            ScaleZRectangle Brush(offset, strength, width, length);
            pBrush = &Brush;
        }
              break;

              // 13 ScaleZCustomized �Զ�����������
        case 3: {
            // ��Ҫ�û������ֵ
            double offset = 1;
            double strength = 1;

            // ����������
            CRhinoGetOption go;
            go.SetCommandPrompt(L"����ScaleZCustomized��ˢ����");
            go.AcceptNothing();

            for (;;)
            {
                // ������Ӧ������ӿ�
                go.ClearCommandOptions();
                int offset_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"offset"), &offset, L"double value", FALSE, -999.9, 999.9);
                int strength_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"scale"), &strength, L"double value", FALSE, -999.9, 999.9);

                // ���̿���
                CRhinoGet::result res = go.GetOption();
                if (res == CRhinoGet::nothing) break;
                if (res == CRhinoGet::cancel) return CRhinoCommand::cancel;
                if (res != CRhinoGet::option) return CRhinoCommand::failure;
                const CRhinoCommandOption* option = go.Option();
                if (nullptr == option) return CRhinoCommand::failure;

                // �û�����
                int option_index = option->m_option_index;
                if (option_index == offset_option_index) continue;
                if (option_index == strength_option_index) continue;

            }

            //��ȡ�Զ�������
            CRhinoGetObject get;
            get.SetCommandPrompt(L"ѡ���Զ�������");
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
                        // ���ɱ�ˢʵ��
                        ScaleZCustomized Brush(offset, strength, customizedcurve);
                        pBrush = &Brush;
                        break;
                    }
                    else
                    {
                        RhinoApp().Print(L"��ѡ��һ����յ�����");
                        return CRhinoCommand::success;
                    }
                }
                else
                {
                    RhinoApp().Print(L"��ѡ��һ������");
                    return CRhinoCommand::success;
                }
            }
        }
              break;
        }
        break;

        // 2 Flatten ѹƽ
    case 2:
        switch (Shape_Index)
        {
            // 20 FlattenCircle Բ��ѹƽ
        case 0: {
            // ��Ҫ�û������ֵ
            double offset = 1;
            double targetheight = 1;
            double radius = 1;

            // ����������
            CRhinoGetOption go;
            go.SetCommandPrompt(L"����FlattenCircle��ˢ����");
            go.AcceptNothing();

            for (;;)
            {
                // ������Ӧ������ӿ�
                go.ClearCommandOptions();
                int offset_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"offset"), &offset, L"double value", FALSE, -999.9, 999.9);
                int targetheight_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"targetheight"), &targetheight, L"double value", FALSE, -999.9, 999.9);
                int radius_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"radius"), &radius, L"double value", FALSE, -999.9, 999.9);

                // ���̿���
                CRhinoGet::result res = go.GetOption();
                if (res == CRhinoGet::nothing) break;
                if (res == CRhinoGet::cancel) return CRhinoCommand::cancel;
                if (res != CRhinoGet::option) return CRhinoCommand::failure;
                const CRhinoCommandOption* option = go.Option();
                if (nullptr == option) return CRhinoCommand::failure;

                // �û�����
                int option_index = option->m_option_index;
                if (option_index == offset_option_index) continue;
                if (option_index == targetheight_option_index) continue;
                if (option_index == radius_option_index) continue;

            }

            // ���ɱ�ˢʵ��
            FlattenCircle Brush(offset, targetheight, radius);
            pBrush = &Brush;
            break;
        }

              // 21 FlattenPolygon �������ѹƽ
        case 1: {
            // ��Ҫ�û������ֵ
            double offset = 1;
            int polynum = 3;
            double targetheight = 1;
            double radius = 1;

            // ����������
            CRhinoGetOption go;
            go.SetCommandPrompt(L"����FlattenPolygon��ˢ����");
            go.AcceptNothing();

            for (;;)
            {
                // ������Ӧ������ӿ�
                go.ClearCommandOptions();
                int offset_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"offset"), &offset, L"double value", FALSE, -999.9, 999.9);
                int targetheight_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"targetheight"), &targetheight, L"double value", FALSE, -999.9, 999.9);
                int radius_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"radius"), &radius, L"double value", FALSE, -999.9, 999.9);
                int polynum_option_index = go.AddCommandOptionInteger(RHCMDOPTNAME(L"polynum"), &polynum, L"integer value", 1, 999);

                // ���̿���
                CRhinoGet::result res = go.GetOption();
                if (res == CRhinoGet::nothing) break;
                if (res == CRhinoGet::cancel) return CRhinoCommand::cancel;
                if (res != CRhinoGet::option) return CRhinoCommand::failure;
                const CRhinoCommandOption* option = go.Option();
                if (nullptr == option) return CRhinoCommand::failure;

                // �û�����
                int option_index = option->m_option_index;
                if (option_index == offset_option_index) continue;
                if (option_index == targetheight_option_index) continue;
                if (option_index == radius_option_index) continue;
                if (option_index == polynum_option_index) continue;

            }

            // ���ɱ�ˢʵ��
            FlattenPolygon Brush(offset, targetheight, radius, polynum);
            pBrush = &Brush;
        }
              break;

              // 22 FlattenRectangle ����ѹƽ
        case 2: {
            // ��Ҫ�û������ֵ
            double offset = 1;
            double targetheight = 1;
            double width = 1;
            double length = 1;

            // ����������
            CRhinoGetOption go;
            go.SetCommandPrompt(L"����FlattenRectangle��ˢ����");
            go.AcceptNothing();

            for (;;)
            {
                // ������Ӧ������ӿ�
                go.ClearCommandOptions();
                int offset_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"offset"), &offset, L"double value", FALSE, -999.9, 999.9);
                int targetheight_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"targetheight"), &targetheight, L"double value", FALSE, -999.9, 999.9);
                int width_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"width"), &width, L"double value", FALSE, -999.9, 999.9);
                int length_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"length"), &length, L"double value", FALSE, -999.9, 999.9);

                // ���̿���
                CRhinoGet::result res = go.GetOption();
                if (res == CRhinoGet::nothing) break;
                if (res == CRhinoGet::cancel) return CRhinoCommand::cancel;
                if (res != CRhinoGet::option) return CRhinoCommand::failure;
                const CRhinoCommandOption* option = go.Option();
                if (nullptr == option) return CRhinoCommand::failure;

                // �û�����
                int option_index = option->m_option_index;
                if (option_index == offset_option_index) continue;
                if (option_index == targetheight_option_index) continue;
                if (option_index == width_option_index) continue;
                if (option_index == length_option_index) continue;

            }

            // ���ɱ�ˢʵ��
            FlattenRectangle Brush(offset, targetheight, width, length);
            pBrush = &Brush;
        }
              break;

              // 23 FlattenCustomized �Զ�������ѹƽ
        case 3: {
            // ��Ҫ�û������ֵ
            double offset = 1;
            double targetheight = 1;

            // ����������
            CRhinoGetOption go;
            go.SetCommandPrompt(L"����FlattenCustomized��ˢ����");
            go.AcceptNothing();

            for (;;)
            {
                // ������Ӧ������ӿ�
                go.ClearCommandOptions();
                int offset_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"offset"), &offset, L"double value", FALSE, -999.9, 999.9);
                int targetheight_option_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"targetheight"), &targetheight, L"double value", FALSE, -999.9, 999.9);

                // ���̿���
                CRhinoGet::result res = go.GetOption();
                if (res == CRhinoGet::nothing) break;
                if (res == CRhinoGet::cancel) return CRhinoCommand::cancel;
                if (res != CRhinoGet::option) return CRhinoCommand::failure;
                const CRhinoCommandOption* option = go.Option();
                if (nullptr == option) return CRhinoCommand::failure;

                // �û�����
                int option_index = option->m_option_index;
                if (option_index == offset_option_index) continue;
                if (option_index == targetheight_option_index) continue;

            }

            //��ȡ�Զ�������
            CRhinoGetObject get;
            get.SetCommandPrompt(L"ѡ���Զ�������");
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
                        // ���ɱ�ˢʵ��
                        FlattenCustomized Brush(offset, targetheight, customizedcurve);
                        pBrush = &Brush;
                        break;
                    }
                    else
                    {
                        RhinoApp().Print(L"��ѡ��һ����յ�����");
                        return CRhinoCommand::success;
                    }
                }
                else
                {
                    RhinoApp().Print(L"��ѡ��һ������");
                    return CRhinoCommand::success;
                }
            }
        }
              break;
        }
        break;
    }

    ////////////// ���廭��֮���ִ��

    // ��ȡ�༭��������
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
    //ȡsurface ��UVֵ
    const int srf_U = nurbs_srf->CVCount(0);
    const int srf_V = nurbs_srf->CVCount(1);
    const int allCVcount = srf_U * srf_V;

    // ���Ƶ�����
    ON_3dPoint* ptrCV = new ON_3dPoint[allCVcount];

    // ��ѡȡ���Ƶ�����
    CRhinoObjRef* ptref = new CRhinoObjRef[allCVcount];

    // �м���̬����
    CRhinoObjRef last_nurbs_surface;

    // �ظ�ִ�б༭����
    for (;;) {

        // ��vector���鱣������Ȧ�ڵĵ����Ȧ�ڵĵ���CV�е�λ��
        std::vector<int> indices_in;
        std::vector<int> indices_out;

        // �����ǰ�Ŀ��Ƶ�
        for (int i = 0; i < srf_U; i++)
        {
            for (int j = 0; j < srf_V; j++)
            {
                int index = i * srf_U + j;
                context.m_doc.DeleteObject(ptref[index]);
            }
        }

        // ��ʾ���еĿ��Ƶ�
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

        // ��ȡ�༭�����
        CRhinoGetObject getpoint;
        getpoint.SetCommandPrompt(L"Select a point");
        getpoint.SetGeometryFilter(CRhinoGetObject::point_object);
        CRhinoGet::result res = getpoint.GetObjects(1, 1);

        if (res != CRhinoGet::object) break;

        const CRhinoObjRef& obj_ref = getpoint.Object(0);
        const ON_Point* pt = obj_ref.Point();

        ON_3dPoint editpoint(*pt);
        getpoint.ClearCommandOptions();

        

        // ��ȡ��������
        ON_NurbsCurve innercurve;
        ON_NurbsCurve outercurve;
        
        if (pBrush->pShape->GetInnerAndOuterCurve(&innercurve, &outercurve, editpoint) != S_OK)
        {
            file << "GetInnerAndOuterCurve function ERROR" << std::endl;
            return CRhinoCommand::failure;
        }

        // չʾ��������
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
        //�����õ�����������Ȧ��Χ���Լ�����������Ȧ֮��ĵĵ��index��

        int inCount = indices_in.size();
        int outCount = indices_out.size();

        //��Ϊ��������
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

        //��1�����޸���Ŀ��Ƶ���������һ���µ����桿
        //��2�������޸�ԭ�����ÿһ�����Ƶ㡿
        // Ŀǰ�õ�2�֡�

        //��for inner points��
        if (pBrush->pOperation->InnerEdit(nurbs_srf, ptrCV, indices_IN, inCount) != S_OK)
        {
            file << "InnerEdit function ERROR" <<std:: endl;
            return CRhinoCommand::failure;
        }
        

        //��for outer points��
        if (OuterEdit(nurbs_srf, &innercurve, &outercurve, ptrCV, indices_OUT, indices_IN, outCount, inCount)!= S_OK)
        {
            file << "OuterEdit function ERROR"<<std::endl;
            return CRhinoCommand::failure;
        }

        delete[] indices_IN;
        delete[] indices_OUT;

        // ɾ����ʼsurface
        context.m_doc.DeleteObject(surface_ref);

        // ɾ����һ��surface
        context.m_doc.DeleteObject(last_nurbs_surface);

        // �õ�ǰ��surface��ʼ��CRhinoObjRef new_nurbs_surface
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
