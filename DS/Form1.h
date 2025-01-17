#pragma once
#include "stdafx.h"
#include "ODE.cpp"

namespace DS {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
    using namespace System::Data;
	using namespace System::Drawing;

	int save = -1;
	
	public ref class Form1 : public System::Windows::Forms::Form
	{
	  private:
		//динамическая система
	    ODE2D ^ds;
		Graphics ^G;
		
		//центр изображения
		double Xcentre; 
		double Ycentre;
		//масштаб (расстояние от центра)
		double dist;
		
		
    private: System::Windows::Forms::Button^  butPrint;
    System::Drawing::Bitmap ^bmp;
	private: System::Windows::Forms::Label^  labelDist;
	private: System::Windows::Forms::NumericUpDown^  numericDist;

	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
			ds = gcnew VanDerPolSpec(panel1->Width, panel1->Height, 1.0, 0.0);//1.0 = а, 0.0 = b
			bmp = ds->getBmp();

			Xcentre = Ycentre = 0.0;
			dist = 2.0;
	    }

	protected:
		/// <summary>
		/// освободить все используемые ресурсы.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	 
	private: System::Windows::Forms::Panel^  panel1;
	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// требуется переменная конструктора.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// озательный метод дл¤ поддержки конструктора - не изменяйте
		/// содержимое данного метода при помощи редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->butPrint = (gcnew System::Windows::Forms::Button());
			this->labelDist = (gcnew System::Windows::Forms::Label());
			this->numericDist = (gcnew System::Windows::Forms::NumericUpDown());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericDist))->BeginInit();
			this->SuspendLayout();
			// 
			// panel1
			// 
			this->panel1->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->panel1->Location = System::Drawing::Point(12, 43);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(500, 500);
			this->panel1->TabIndex = 1;
			this->panel1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::panel1_Paint);
			this->panel1->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::panel1_MouseClick);
			// 
			// butPrint
			// 
			this->butPrint->Location = System::Drawing::Point(202, 8);
			this->butPrint->Name = L"butPrint";
			this->butPrint->Size = System::Drawing::Size(310, 19);
			this->butPrint->TabIndex = 2;
			this->butPrint->Text = L"Масштабировать";
			this->butPrint->UseVisualStyleBackColor = true;
			this->butPrint->Click += gcnew System::EventHandler(this, &Form1::butPrint_Click);
			// 
			// labelDist
			// 
			this->labelDist->AutoSize = true;
			this->labelDist->Location = System::Drawing::Point(12, 9);
			this->labelDist->Name = L"labelDist";
			this->labelDist->Size = System::Drawing::Size(56, 13);
			this->labelDist->TabIndex = 8;
			this->labelDist->Text = L"Масштаб:";
			// 
			// numericDist
			// 
			this->numericDist->DecimalPlaces = 15;
			this->numericDist->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 983040});
			this->numericDist->Location = System::Drawing::Point(71, 7);
			this->numericDist->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 983040});
			this->numericDist->Name = L"numericDist";
			this->numericDist->Size = System::Drawing::Size(125, 20);
			this->numericDist->TabIndex = 7;
			this->numericDist->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {2, 0, 0, 0});
			this->numericDist->ValueChanged += gcnew System::EventHandler(this, &Form1::numericDist_ValueChanged);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(526, 557);
			this->Controls->Add(this->butPrint);
			this->Controls->Add(this->numericDist);
			this->Controls->Add(this->labelDist);
			this->Controls->Add(this->panel1);
			this->Name = L"Form1";
			this->Text = L"Фазовый портрет";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericDist))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

private: 
  System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
  { this->Close(); }
  
private: System::Void butPrint_Click(System::Object^  sender, System::EventArgs^  e) {
		    save = 1;
			ds->setDist(dist);
			this->Refresh();//перерисовывание при изменении масштаба
		 }

private: System::Void panel1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
			G = e->Graphics;
			ds->updateBitmap();
			G->DrawImage(bmp, 0, 0);
		 }

private: System::Void numericDist_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 dist = Decimal::ToDouble(numericDist->Value);
	     }

private: System::Void panel1_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			Double dX =  System::Convert::ToDouble(e->X);
			Double dY =  System::Convert::ToDouble(e->Y);
			ds->setX0(dX, dY);
			ds->rk4();

		    this->Refresh();
         }
};
}