#pragma once

#include "SoundManager.h"

namespace Sudoku {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Diagnostics;

	public ref class AboutForm sealed : public System::Windows::Forms::Form {
	public:
		AboutForm() {
			InitializeComponent();
		}
	protected:
		~AboutForm() {
			if (components) {
				delete components;
			}
		}
	private:
		System::Windows::Forms::PictureBox^ pbIcon;
		System::Windows::Forms::Label^ labelTitle;
		System::Windows::Forms::Label^ labelAuthor;
		System::Windows::Forms::WebBrowser^ wbMedia;
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent()
		{
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(AboutForm::typeid));
			this->pbIcon = (gcnew System::Windows::Forms::PictureBox());
			this->labelTitle = (gcnew System::Windows::Forms::Label());
			this->labelAuthor = (gcnew System::Windows::Forms::Label());
			this->wbMedia = (gcnew System::Windows::Forms::WebBrowser());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbIcon))->BeginInit();
			this->SuspendLayout();
			//
			// pbIcon
			//
			this->pbIcon->BackColor = System::Drawing::Color::Transparent;
			this->pbIcon->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pbIcon.Image")));
			this->pbIcon->Location = System::Drawing::Point(12, 12);
			this->pbIcon->Name = L"pbIcon";
			this->pbIcon->Size = System::Drawing::Size(100, 100);
			this->pbIcon->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pbIcon->TabIndex = 0;
			this->pbIcon->TabStop = false;
			this->pbIcon->Click += gcnew System::EventHandler(this, &AboutForm::pbIcon_Click);
			//
			// labelTitle
			//
			this->labelTitle->AutoSize = true;
			this->labelTitle->BackColor = System::Drawing::Color::Transparent;
			this->labelTitle->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->labelTitle->ForeColor = System::Drawing::Color::Purple;
			this->labelTitle->Location = System::Drawing::Point(135, 12);
			this->labelTitle->Name = L"labelTitle";
			this->labelTitle->Size = System::Drawing::Size(149, 40);
			this->labelTitle->TabIndex = 0;
			this->labelTitle->Text = L"數獨遊戲\r\n棉被正在看著你 ><";
			//
			// labelAuthor
			//
			this->labelAuthor->AutoSize = true;
			this->labelAuthor->BackColor = System::Drawing::Color::Transparent;
			this->labelAuthor->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->labelAuthor->ForeColor = System::Drawing::Color::Black;
			this->labelAuthor->Location = System::Drawing::Point(498, 65);
			this->labelAuthor->Name = L"labelAuthor";
			this->labelAuthor->Size = System::Drawing::Size(124, 80);
			this->labelAuthor->TabIndex = 1;
			this->labelAuthor->Text = L"組員\r\n陳鈞翔 1082937\r\n袁忻姵 1082924\r\n棉被 (+ 小棉被)";
			this->labelAuthor->TextAlign = System::Drawing::ContentAlignment::TopRight;
			//
			// wbMedia
			//
			this->wbMedia->IsWebBrowserContextMenuEnabled = false;
			this->wbMedia->Location = System::Drawing::Point(-9, -12);
			this->wbMedia->MinimumSize = System::Drawing::Size(20, 20);
			this->wbMedia->Name = L"wbMedia";
			this->wbMedia->ScrollBarsEnabled = false;
			this->wbMedia->Size = System::Drawing::Size(650, 375);
			this->wbMedia->TabIndex = 2;
			this->wbMedia->Url = (gcnew System::Uri(L"", System::UriKind::Relative));
			this->wbMedia->Visible = false;
			this->wbMedia->WebBrowserShortcutsEnabled = false;
			//
			// AboutForm
			//
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->ClientSize = System::Drawing::Size(634, 356);
			this->Controls->Add(this->wbMedia);
			this->Controls->Add(this->labelAuthor);
			this->Controls->Add(this->labelTitle);
			this->Controls->Add(this->pbIcon);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->MaximizeBox = false;
			this->Name = L"AboutForm";
			this->Text = L"關於";
			this->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &AboutForm::AboutForm_MouseClick);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbIcon))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();
		}
#pragma endregion

	private:
		System::Void pbIcon_Click(System::Object^ sender, System::EventArgs^ e) {
			Process::Start("https://intro.quilt.idv.tw/");
		}

		System::Void AboutForm_MouseClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
			if (!SoundManager::getInstance()->isMuteBGM()) {
				SoundManager::getInstance()->toggleMuteBGM();
			}
			wbMedia->Url = gcnew Uri("https://smallquilt.quilt.idv.tw:8923/resource2.php");
			wbMedia->Visible = true;
		}
	};
}