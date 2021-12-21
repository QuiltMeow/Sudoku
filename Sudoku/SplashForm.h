#pragma once

namespace Sudoku {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class SplashForm sealed : public System::Windows::Forms::Form {
	public:
		SplashForm() {
			InitializeComponent();
		}
	protected:
		~SplashForm() {
			if (components) {
				delete components;
			}
		}
	private:
		System::Windows::Forms::ProgressBar^ progressBar;
		System::Windows::Forms::Timer^ timerProgress;
		System::Windows::Forms::Label^ labelTitle;
		System::ComponentModel::IContainer^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent()
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(SplashForm::typeid));
			this->progressBar = (gcnew System::Windows::Forms::ProgressBar());
			this->timerProgress = (gcnew System::Windows::Forms::Timer(this->components));
			this->labelTitle = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			//
			// progressBar
			//
			this->progressBar->ForeColor = System::Drawing::Color::Violet;
			this->progressBar->Location = System::Drawing::Point(125, 365);
			this->progressBar->Name = L"progressBar";
			this->progressBar->Size = System::Drawing::Size(300, 35);
			this->progressBar->TabIndex = 1;
			//
			// timerProgress
			//
			this->timerProgress->Enabled = true;
			this->timerProgress->Interval = 32;
			this->timerProgress->Tick += gcnew System::EventHandler(this, &SplashForm::timerProgress_Tick);
			//
			// labelTitle
			//
			this->labelTitle->AutoSize = true;
			this->labelTitle->BackColor = System::Drawing::Color::Transparent;
			this->labelTitle->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->labelTitle->ForeColor = System::Drawing::Color::Khaki;
			this->labelTitle->Location = System::Drawing::Point(15, 25);
			this->labelTitle->Name = L"labelTitle";
			this->labelTitle->Size = System::Drawing::Size(125, 48);
			this->labelTitle->TabIndex = 0;
			this->labelTitle->Text = L"數獨遊戲\r\n正在載入中 ...";
			//
			// SplashForm
			//
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->ClientSize = System::Drawing::Size(600, 450);
			this->Controls->Add(this->labelTitle);
			this->Controls->Add(this->progressBar);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"SplashForm";
			this->ShowIcon = false;
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"起始畫面";
			this->TopMost = true;
			this->ResumeLayout(false);
			this->PerformLayout();
		}
#pragma endregion

	private:
		System::Void timerProgress_Tick(System::Object^ sender, System::EventArgs^ e) {
			progressBar->Increment(1);
			if (progressBar->Value >= 100) {
				timerProgress->Stop();
			}
		}
	};
}