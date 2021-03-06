#pragma once

#include "antares.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
//extern "C" {
//#include "FBLib_rec.h"
//}


namespace Antares {

	/// <summary>
	/// Summary for ProgInfo
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class ProgInfo : public System::Windows::Forms::Form
	{
	public:
		ProgInfo(void)
		{
			InitializeComponent();

		}

		System::Windows::Forms::RadioButton^ make_toggle_button(void)
		{
			System::Windows::Forms::RadioButton^ b = gcnew(RadioButton);
		    b->Appearance = System::Windows::Forms::Appearance::Button;
			b->AutoSize = true;
			b->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			b->ForeColor = System::Drawing::Color::Black;
			b->Location = System::Drawing::Point(221, 235);
			//this->radioButton2->Name = L"radioButton2";
			b->Size = System::Drawing::Size(25, 24);
			//this->radioButton2->TabIndex = 21;
			b->Text = "";
			b->UseVisualStyleBackColor = true;
			b->CheckedChanged += gcnew System::EventHandler(this, &ProgInfo::radioButton_CheckedChanged);

			return b;
		}

		ProgInfo( array<MyStuffInfo^>^ mia, FileItem ^ item, String^ windowtitle)
		{
			InitializeComponent();
			this->apply_language();
			this->current_index=-1;
			this->mia = mia;
			this->filename->Text = windowtitle;

			if (this->filename->Width > this->Width)
				this->filename->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Bold));
			

			this->item = item;

			int n = mia->Length;
			int i1,i2;
			if (n==1) 
			{
				i1=0;i2=0;
			}
			else
			{
				i1=1; i2=n-1;
			}
			int num = i2-i1+1;

			int x=this->description->Location.X;
			int y=235;

			for (int i=i1; i<=i2; i++)
			{
				RadioButton ^b = this->make_toggle_button();
				this->Controls->Add(b);
				b->Show();
				Point p = b->Location;
				p.X = x;
				b->Location = p;

				b->Text = i.ToString();
				b->Tag = i;

				if (i==i1) b->Checked = true;

				x = x + b->Size.Width + 5;
				if (i1==i2) b->Visible=false;
			}
			this->PerformLayout();





		}

		ProgInfo(tRECHeaderInfo *ri, String^ windowtitle)
		{
			InitializeComponent();
			this->apply_language();

			String^ channel = gcnew String(ri->SISvcName);
			String^ title = gcnew String(ri->EventEventName);
			String^ description = gcnew String(ri->EventEventDescription);
			String^ ext = gcnew String(ri->ExtEventText);
			if (description->Length >0 && ext->Length >0 )
			{
				description = description + "\r\n  --- \r\n";
			}
			description = description + ext;

			this->filename->Text = windowtitle;// "Program Information, "+fname;
			this->channel->Text = channel;
			this->title->Text = title;
			this->description->Text = description;
			String^ duration = ri->EventDurationMin.ToString() +lang::u_minutes;
			if (ri->EventDurationHour > 0 ) duration =  ri->EventDurationHour.ToString() + lang::u_hours+" " + duration;
			this->duration->Text = duration;
			int recorded_min = ri->HeaderDuration; 
			String^ recorded_duration = (recorded_min % 60).ToString() +lang::u_minutes;
			int recorded_hr = recorded_min/60;
			if (recorded_hr>0) recorded_duration = recorded_hr.ToString() + lang::u_hours+" " + recorded_duration;
			this->recorded_duration->Text = recorded_duration;
           
		}

		void set_current_index(int i)
		{
			this->current_index=i;
			MyStuffInfo^ m = mia[i];
			this->channel->Text = item->channel;
			this->title->Text = m->title;
			this->description->Text = m->description;

			int recorded_min = item->reclen;
			String^ recorded_duration = (recorded_min % 60).ToString() +lang::u_minutes;
			int recorded_hr = recorded_min/60;
			if (recorded_hr>0) recorded_duration = recorded_hr.ToString() + lang::u_hours+" " + recorded_duration;
			this->recorded_duration->Text = recorded_duration;

		
	        int prog_min = 	m->proglen;
			String^ prog_duration = (prog_min % 60).ToString() +lang::u_minutes;
			int prog_hr = prog_min/60;
			if (prog_hr>0) prog_duration = prog_hr.ToString() + lang::u_hours+" " + prog_duration;
			this->duration->Text = prog_duration;



		}

		static void right_align_control(System::Windows::Forms::Control ^ c, int r)
		{
			Drawing::Point p = c->Location;
			p.X = r - c->Width;
			c->Location = p;
		}

		void apply_language(void)
		{
			this->label2->Text = lang::h_channel+":";
			this->label3->Text = lang::p_title;
			this->label4->Text = lang::h_description+":";
			this->label5->Text = lang::p_proglen;
			this->label6->Text = lang::p_reclen;
			this->button1->Text = lang::b_close;
			this->Name = lang::p_wintitle;
			this->Text = lang::p_wintitle;
			

			right_align_control(this->label2,159);
			right_align_control(this->label3,159);
			right_align_control(this->label4,159);
			right_align_control(this->label5,159);

			right_align_control(this->label6,590);


		}


	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ProgInfo()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::Label^  label2;
	protected: 
	public: System::Windows::Forms::Label^  label3;
	public: System::Windows::Forms::Label^  label4;
	public: System::Windows::Forms::Label^  filename;
	public: System::Windows::Forms::Label^  channel;
	public: System::Windows::Forms::Label^  title;
	public: System::Windows::Forms::TextBox^  description;
	public: System::Windows::Forms::Label^  label5;
	public: System::Windows::Forms::Panel^  panel1;
	public: System::Windows::Forms::Label^  duration;
	public: System::Windows::Forms::Button^  button1;
	public: System::Windows::Forms::Label^  recorded_duration;

	public: System::Windows::Forms::Label^  label6;

	public: 





			 int current_index;
			 array<MyStuffInfo^>^ mia;
			 Antares::FileItem ^item;



	public: 

	protected: 






	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->filename = (gcnew System::Windows::Forms::Label());
			this->channel = (gcnew System::Windows::Forms::Label());
			this->title = (gcnew System::Windows::Forms::Label());
			this->description = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->duration = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->recorded_duration = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->panel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// label2
			// 
			this->label2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label2->ForeColor = System::Drawing::Color::DarkGreen;
			this->label2->Location = System::Drawing::Point(87, 46);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(74, 18);
			this->label2->TabIndex = 1;
			this->label2->Text = L"Channel:";
			// 
			// label3
			// 
			this->label3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label3->ForeColor = System::Drawing::Color::DarkGreen;
			this->label3->Location = System::Drawing::Point(114, 72);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(45, 18);
			this->label3->TabIndex = 2;
			this->label3->Text = L"Title:";
			// 
			// label4
			// 
			this->label4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label4->ForeColor = System::Drawing::Color::DarkGreen;
			this->label4->Location = System::Drawing::Point(68, 124);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(99, 18);
			this->label4->TabIndex = 3;
			this->label4->Text = L"Description:";
			// 
			// filename
			// 
			this->filename->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->filename->AutoSize = true;
			this->filename->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11.25F, System::Drawing::FontStyle::Bold));
			this->filename->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(80)), static_cast<System::Int32>(static_cast<System::Byte>(80)), 
				static_cast<System::Int32>(static_cast<System::Byte>(80)));
			this->filename->Location = System::Drawing::Point(177, 4);
			this->filename->Name = L"filename";
			this->filename->Size = System::Drawing::Size(341, 18);
			this->filename->TabIndex = 4;
			this->filename->Text = L"Program information,   c:\\some filename.rec";
			// 
			// channel
			// 
			this->channel->AutoSize = true;
			this->channel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->channel->ForeColor = System::Drawing::Color::Navy;
			this->channel->Location = System::Drawing::Point(172, 46);
			this->channel->Name = L"channel";
			this->channel->Size = System::Drawing::Size(106, 18);
			this->channel->TabIndex = 5;
			this->channel->Text = L"Some Channel";
			// 
			// title
			// 
			this->title->AutoSize = true;
			this->title->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->title->ForeColor = System::Drawing::Color::Navy;
			this->title->Location = System::Drawing::Point(172, 72);
			this->title->Name = L"title";
			this->title->Size = System::Drawing::Size(178, 18);
			this->title->TabIndex = 6;
			this->title->Text = L"A really good show, part 2";
			// 
			// description
			// 
			this->description->BackColor = System::Drawing::Color::GhostWhite;
			this->description->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11.5F));
			this->description->ForeColor = System::Drawing::Color::Black;
			this->description->Location = System::Drawing::Point(175, 124);
			this->description->Margin = System::Windows::Forms::Padding(5);
			this->description->Multiline = true;
			this->description->Name = L"description";
			this->description->ReadOnly = true;
			this->description->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->description->Size = System::Drawing::Size(502, 106);
			this->description->TabIndex = 7;
			this->description->TabStop = false;
			this->description->Text = L"In this episode, some really interesting stuff happens. Starring Joe Blogs, Mary " 
				L"Smith. 2010.";
			// 
			// label5
			// 
			this->label5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label5->ForeColor = System::Drawing::Color::DarkGreen;
			this->label5->Location = System::Drawing::Point(33, 98);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(128, 18);
			this->label5->TabIndex = 8;
			this->label5->Text = L"Program length:";
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(240)), static_cast<System::Int32>(static_cast<System::Byte>(240)), 
				static_cast<System::Int32>(static_cast<System::Byte>(255)));
			this->panel1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panel1->Controls->Add(this->filename);
			this->panel1->Dock = System::Windows::Forms::DockStyle::Top;
			this->panel1->Location = System::Drawing::Point(0, 0);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(689, 31);
			this->panel1->TabIndex = 9;
			this->panel1->Layout += gcnew System::Windows::Forms::LayoutEventHandler(this, &ProgInfo::panel1_Layout);
			// 
			// duration
			// 
			this->duration->AutoSize = true;
			this->duration->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->duration->ForeColor = System::Drawing::Color::Navy;
			this->duration->Location = System::Drawing::Point(172, 98);
			this->duration->Name = L"duration";
			this->duration->Size = System::Drawing::Size(73, 18);
			this->duration->TabIndex = 10;
			this->duration->Text = L"1hr 45min";
			// 
			// button1
			// 
			this->button1->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->button1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button1->ForeColor = System::Drawing::SystemColors::ControlText;
			this->button1->Location = System::Drawing::Point(36, 174);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(123, 26);
			this->button1->TabIndex = 11;
			this->button1->Text = L"Close";
			this->button1->UseVisualStyleBackColor = true;
			// 
			// recorded_duration
			// 
			this->recorded_duration->AutoSize = true;
			this->recorded_duration->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11.25F, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->recorded_duration->ForeColor = System::Drawing::Color::Navy;
			this->recorded_duration->Location = System::Drawing::Point(593, 98);
			this->recorded_duration->Name = L"recorded_duration";
			this->recorded_duration->Size = System::Drawing::Size(73, 18);
			this->recorded_duration->TabIndex = 13;
			this->recorded_duration->Text = L"1hr 45min";
			// 
			// label6
			// 
			this->label6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->label6->AutoSize = true;
			this->label6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label6->ForeColor = System::Drawing::Color::DarkGreen;
			this->label6->Location = System::Drawing::Point(451, 98);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(136, 18);
			this->label6->TabIndex = 12;
			this->label6->Text = L"Recorded length:";
			// 
			// ProgInfo
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::GhostWhite;
			this->ClientSize = System::Drawing::Size(689, 266);
			this->Controls->Add(this->recorded_duration);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->duration);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->description);
			this->Controls->Add(this->title);
			this->Controls->Add(this->channel);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->ForeColor = System::Drawing::Color::Green;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Name = L"ProgInfo";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"Program Information";
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void panel1_Layout(System::Object^  sender, System::Windows::Forms::LayoutEventArgs^  e) {
				 int tw = this->filename->Width;
				 int pw = this->panel1->Width;
				 Point p = this->filename->Location;
				 int x = (pw-tw)/2;
				 //this->filename->Font->
				 if (x<0) {x=0;}
				 p.X = x;
				 this->filename->Location = p;
			 }
private: System::Void radioButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

			 RadioButton^ b = safe_cast<RadioButton^>(sender);

			 if (b->Checked == true)
			 {
				 int i = (int) b->Tag;
				 if (this->current_index != i)
				 {
					 

					 this->set_current_index(i);


				 }
			 }


		 }
};
}
