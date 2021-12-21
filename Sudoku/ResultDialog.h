#pragma once

#include "SudokuLevel.h"
#include "ScoreBoard.h"
#include "Util.h"
#include "SoundManager.h"
#include "ResourceHandler.h"

namespace Sudoku {
	using namespace System;
	using namespace System::Text;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace MapleUI;

	public ref class ResultDialog sealed : public System::Windows::Forms::Form {
	public:
		ResultDialog(int score, int solveTime, SudokuLevel level) {
			InitializeComponent();
			starPicture = gcnew array<PictureBox^> { pbStar1, pbStar2, pbStar3 };

			this->score = score;
			this->solveTime = solveTime;
			this->level = level;
			loadNewRecordControl();
		}
	protected:
		static ScoreBoard^ scoreBoard = ScoreBoard::getInstance();
		static SoundManager^ sound = SoundManager::getInstance();
		static ResourceHandler^ resource = ResourceHandler::getInstance();

		int score, solveTime;
		SudokuLevel level;
		array<PictureBox^>^ starPicture;

		~ResultDialog() {
			if (components) {
				delete components;
			}
		}
	private:
		System::Windows::Forms::Button^ btnDone;
		System::Windows::Forms::TextBox^ txtName;
		System::Windows::Forms::Label^ labelTip;
		System::Windows::Forms::PictureBox^ pbStar1;
		System::Windows::Forms::PictureBox^ pbStar2;
		System::Windows::Forms::PictureBox^ pbStar3;
		System::Windows::Forms::Label^ labelRecord;
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent()
		{
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(ResultDialog::typeid));
			this->btnDone = (gcnew System::Windows::Forms::Button());
			this->txtName = (gcnew System::Windows::Forms::TextBox());
			this->labelTip = (gcnew System::Windows::Forms::Label());
			this->pbStar1 = (gcnew System::Windows::Forms::PictureBox());
			this->pbStar2 = (gcnew System::Windows::Forms::PictureBox());
			this->pbStar3 = (gcnew System::Windows::Forms::PictureBox());
			this->labelRecord = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbStar1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbStar2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbStar3))->BeginInit();
			this->SuspendLayout();
			//
			// btnDone
			//
			this->btnDone->Location = System::Drawing::Point(338, 264);
			this->btnDone->Name = L"btnDone";
			this->btnDone->Size = System::Drawing::Size(75, 21);
			this->btnDone->TabIndex = 3;
			this->btnDone->Text = L"完成";
			this->btnDone->UseVisualStyleBackColor = true;
			this->btnDone->Visible = false;
			this->btnDone->Click += gcnew System::EventHandler(this, &ResultDialog::btnDone_Click);
			//
			// txtName
			//
			this->txtName->Location = System::Drawing::Point(167, 265);
			this->txtName->Name = L"txtName";
			this->txtName->Size = System::Drawing::Size(165, 22);
			this->txtName->TabIndex = 2;
			this->txtName->Visible = false;
			//
			// labelTip
			//
			this->labelTip->AutoSize = true;
			this->labelTip->BackColor = System::Drawing::Color::Transparent;
			this->labelTip->ForeColor = System::Drawing::Color::White;
			this->labelTip->Location = System::Drawing::Point(165, 245);
			this->labelTip->Name = L"labelTip";
			this->labelTip->Size = System::Drawing::Size(185, 12);
			this->labelTip->TabIndex = 1;
			this->labelTip->Text = L"恭喜您刷新紀錄，請輸入您的名字";
			this->labelTip->Visible = false;
			//
			// pbStar1
			//
			this->pbStar1->BackColor = System::Drawing::Color::Transparent;
			this->pbStar1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pbStar1.Image")));
			this->pbStar1->Location = System::Drawing::Point(75, 50);
			this->pbStar1->Name = L"pbStar1";
			this->pbStar1->Size = System::Drawing::Size(100, 100);
			this->pbStar1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pbStar1->TabIndex = 3;
			this->pbStar1->TabStop = false;
			//
			// pbStar2
			//
			this->pbStar2->BackColor = System::Drawing::Color::Transparent;
			this->pbStar2->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pbStar2.Image")));
			this->pbStar2->Location = System::Drawing::Point(240, 20);
			this->pbStar2->Name = L"pbStar2";
			this->pbStar2->Size = System::Drawing::Size(100, 100);
			this->pbStar2->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pbStar2->TabIndex = 4;
			this->pbStar2->TabStop = false;
			//
			// pbStar3
			//
			this->pbStar3->BackColor = System::Drawing::Color::Transparent;
			this->pbStar3->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pbStar3.Image")));
			this->pbStar3->Location = System::Drawing::Point(405, 50);
			this->pbStar3->Name = L"pbStar3";
			this->pbStar3->Size = System::Drawing::Size(100, 100);
			this->pbStar3->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pbStar3->TabIndex = 5;
			this->pbStar3->TabStop = false;
			//
			// labelRecord
			//
			this->labelRecord->AutoSize = true;
			this->labelRecord->BackColor = System::Drawing::Color::Transparent;
			this->labelRecord->ForeColor = System::Drawing::Color::White;
			this->labelRecord->Location = System::Drawing::Point(200, 155);
			this->labelRecord->Name = L"labelRecord";
			this->labelRecord->Size = System::Drawing::Size(53, 12);
			this->labelRecord->TabIndex = 0;
			this->labelRecord->Text = L"分數紀錄";
			//
			// ResultDialog
			//
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->ClientSize = System::Drawing::Size(584, 346);
			this->Controls->Add(this->labelRecord);
			this->Controls->Add(this->pbStar3);
			this->Controls->Add(this->pbStar2);
			this->Controls->Add(this->pbStar1);
			this->Controls->Add(this->btnDone);
			this->Controls->Add(this->txtName);
			this->Controls->Add(this->labelTip);
			this->DoubleBuffered = true;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->MaximizeBox = false;
			this->Name = L"ResultDialog";
			this->Text = L"遊戲成績";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbStar1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbStar2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbStar3))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();
		}
#pragma endregion

	private:
		void loadNewRecordControl() {
			int star = calculateStar();
			for (int i = 0; i < 3; ++i) {
				starPicture[i]->Visible = star >= i + 1;
			}

			StringBuilder^ sb = gcnew StringBuilder();
			sb->Append("分數 : ")->AppendLine(score.ToString());
			sb->Append("解題時間 : ")->AppendLine(makeTimeReadable(solveTime));
			sb->Append("解題難度 : ")->AppendLine(getLevelName(level));
			labelRecord->Text = sb->ToString();

			if (scoreBoard->canAdd(score, solveTime, level)) {
				visibleNewRecord(true);
				sound->getSoundPlayer()->Stream = resource->getGameResource()->GetStream("NewRecord");
				sound->getSoundPlayer()->Play();
			}
		}

		void visibleNewRecord(bool visible) {
			labelTip->Visible = visible;
			txtName->Visible = visible;
			btnDone->Visible = visible;
		}

		System::Void btnDone_Click(System::Object^ sender, System::EventArgs^ e) {
			String^ name = txtName->Text->Trim(' ');
			if (name == "") {
				(gcnew MapleMessageBox("請輸入您的名字"))->Show();
				return;
			}
			if (name->Length > ScoreBoard::getMaxNameLength()) {
				(gcnew MapleMessageBox("名稱過長" + Environment::NewLine + "請重新輸入"))->Show();
				return;
			}
			scoreBoard->addPlayer(gcnew PlayerInformation(name, score, solveTime, level));
			Close();
		}

		int calculateStar() {
			if (score >= 11000) {
				return 3;
			}
			else if (score >= 9000) {
				return 2;
			}
			return 1;
		}
	public:
		void applyFont() {
			System::Drawing::Font^ displayTitle = resource->getDisplayTitleFont();
			System::Drawing::Font^ display = resource->getDisplayContentFont();
			labelRecord->Font = displayTitle;
			labelTip->Font = display;
			txtName->Font = display;
			btnDone->Font = display;
		}
	};
}