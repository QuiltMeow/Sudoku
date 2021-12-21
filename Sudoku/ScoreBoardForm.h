#pragma once

#include "ScoreBoard.h"
#include "PlayerInformation.h"
#include "EWException.h"
#include "Util.h"
#include "ResourceHandler.h"

namespace Sudoku {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class ScoreBoardForm sealed : public System::Windows::Forms::Form {
	public:
		ScoreBoardForm() {
			InitializeComponent();
			loadScoreBoard();
		}
	protected:
		static ScoreBoard^ score = ScoreBoard::getInstance();

		~ScoreBoardForm() {
			if (components) {
				delete components;
			}
		}
	private:
		System::Windows::Forms::Label^ labelTitle;
		System::Windows::Forms::ColumnHeader^ chRank;
		System::Windows::Forms::ColumnHeader^ chName;
		System::Windows::Forms::ColumnHeader^ chScore;
		System::Windows::Forms::ColumnHeader^ chSolveTime;
		System::Windows::Forms::ColumnHeader^ chSolveLevel;
		System::Windows::Forms::Button^ btnDeleteAll;
		System::Windows::Forms::ListView^ lvScore;
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent()
		{
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(ScoreBoardForm::typeid));
			this->lvScore = (gcnew System::Windows::Forms::ListView());
			this->chRank = (gcnew System::Windows::Forms::ColumnHeader());
			this->chName = (gcnew System::Windows::Forms::ColumnHeader());
			this->chScore = (gcnew System::Windows::Forms::ColumnHeader());
			this->chSolveTime = (gcnew System::Windows::Forms::ColumnHeader());
			this->chSolveLevel = (gcnew System::Windows::Forms::ColumnHeader());
			this->labelTitle = (gcnew System::Windows::Forms::Label());
			this->btnDeleteAll = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			//
			// lvScore
			//
			this->lvScore->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(5) {
				this->chRank, this->chName,
					this->chScore, this->chSolveTime, this->chSolveLevel
			});
			this->lvScore->GridLines = true;
			this->lvScore->HideSelection = false;
			this->lvScore->Location = System::Drawing::Point(30, 40);
			this->lvScore->Name = L"lvScore";
			this->lvScore->Size = System::Drawing::Size(490, 255);
			this->lvScore->TabIndex = 1;
			this->lvScore->UseCompatibleStateImageBehavior = false;
			this->lvScore->View = System::Windows::Forms::View::Details;
			//
			// chRank
			//
			this->chRank->Text = L"排名";
			//
			// chName
			//
			this->chName->Text = L"名稱";
			this->chName->Width = 150;
			//
			// chScore
			//
			this->chScore->Text = L"分數";
			this->chScore->Width = 100;
			//
			// chSolveTime
			//
			this->chSolveTime->Text = L"解題時間";
			this->chSolveTime->Width = 100;
			//
			// chSolveLevel
			//
			this->chSolveLevel->Text = L"解題難度";
			this->chSolveLevel->Width = 70;
			//
			// labelTitle
			//
			this->labelTitle->AutoSize = true;
			this->labelTitle->Location = System::Drawing::Point(12, 9);
			this->labelTitle->Name = L"labelTitle";
			this->labelTitle->Size = System::Drawing::Size(53, 12);
			this->labelTitle->TabIndex = 0;
			this->labelTitle->Text = L"高分紀錄";
			//
			// btnDeleteAll
			//
			this->btnDeleteAll->Location = System::Drawing::Point(432, 311);
			this->btnDeleteAll->Name = L"btnDeleteAll";
			this->btnDeleteAll->Size = System::Drawing::Size(105, 23);
			this->btnDeleteAll->TabIndex = 2;
			this->btnDeleteAll->Text = L"清除所有紀錄";
			this->btnDeleteAll->UseVisualStyleBackColor = true;
			this->btnDeleteAll->Click += gcnew System::EventHandler(this, &ScoreBoardForm::btnDeleteAll_Click);
			//
			// ScoreBoardForm
			//
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(549, 346);
			this->Controls->Add(this->btnDeleteAll);
			this->Controls->Add(this->labelTitle);
			this->Controls->Add(this->lvScore);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->MaximizeBox = false;
			this->Name = L"ScoreBoardForm";
			this->Text = L"高分紀錄";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &ScoreBoardForm::ScoreBoardForm_FormClosing);
			this->ResumeLayout(false);
			this->PerformLayout();
		}
#pragma endregion

	private:
		System::Void btnDeleteAll_Click(System::Object^ sender, System::EventArgs^ e) {
			if (MessageBox::Show("請問是否清除所有紀錄 ?", "確認", MessageBoxButtons::YesNo, MessageBoxIcon::Question) == System::Windows::Forms::DialogResult::Yes) {
				score->clear();
				loadScoreBoard();
			}
		}

		System::Void ScoreBoardForm_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e) {
			Hide();
			e->Cancel = true;
		}
	public:
		void loadScoreBoard() {
			lvScore->Items->Clear();
			System::Collections::Generic::IList<PlayerInformation^>^ infoList = score->getPlayerData();
			for (int i = 0; i < infoList->Count; ++i) {
				PlayerInformation^ info = infoList[i];
				array<String^>^ data = gcnew array<String^> { (i + 1).ToString(), info->getName(), info->getScore().ToString(), makeTimeReadable(info->getSolveTime()), getLevelName(info->getSolveLevel()) };
				ListViewItem^ item = gcnew ListViewItem(data);
				lvScore->Items->Add(item);
			}
		}

		void applyFont() {
			System::Drawing::Font^ displayTitle = ResourceHandler::getInstance()->getDisplayTitleFont();
			System::Drawing::Font^ displayContent = ResourceHandler::getInstance()->getDisplayContentFont();
			labelTitle->Font = displayTitle;
			btnDeleteAll->Font = displayContent;
		}
	};
}