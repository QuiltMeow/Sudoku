#pragma once

#include "ResourceHandler.h"

namespace Sudoku {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class HelpForm sealed : public System::Windows::Forms::Form {
	public:
		HelpForm() {
			InitializeComponent();
		}
	protected:
		~HelpForm() {
			if (components) {
				delete components;
			}
		}
	private:
		System::Windows::Forms::Label^ labelIntroduction;
		System::Windows::Forms::Label^ labelIntroductionTitle;
		System::Windows::Forms::Label^ labelRuleTitle;
		System::Windows::Forms::Label^ labelRule;
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改
		/// 這個方法的內容。
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(HelpForm::typeid));
			this->labelIntroduction = (gcnew System::Windows::Forms::Label());
			this->labelIntroductionTitle = (gcnew System::Windows::Forms::Label());
			this->labelRuleTitle = (gcnew System::Windows::Forms::Label());
			this->labelRule = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			//
			// labelIntroduction
			//
			this->labelIntroduction->AutoSize = true;
			this->labelIntroduction->Location = System::Drawing::Point(25, 40);
			this->labelIntroduction->Name = L"labelIntroduction";
			this->labelIntroduction->Size = System::Drawing::Size(293, 36);
			this->labelIntroduction->TabIndex = 1;
			this->labelIntroduction->Text = L"數獨是一種數學邏輯遊戲，遊戲由 9 × 9 個格子組成，\r\n玩家需要根據格子提供的數字推理出其他格子的數字。\r\n題目提供最少 17 個數字使得解答謎題只有一個答案"
				L"。";
			//
			// labelIntroductionTitle
			//
			this->labelIntroductionTitle->AutoSize = true;
			this->labelIntroductionTitle->Location = System::Drawing::Point(12, 9);
			this->labelIntroductionTitle->Name = L"labelIntroductionTitle";
			this->labelIntroductionTitle->Size = System::Drawing::Size(29, 12);
			this->labelIntroductionTitle->TabIndex = 0;
			this->labelIntroductionTitle->Text = L"說明";
			//
			// labelRuleTitle
			//
			this->labelRuleTitle->AutoSize = true;
			this->labelRuleTitle->Location = System::Drawing::Point(12, 115);
			this->labelRuleTitle->Name = L"labelRuleTitle";
			this->labelRuleTitle->Size = System::Drawing::Size(29, 12);
			this->labelRuleTitle->TabIndex = 2;
			this->labelRuleTitle->Text = L"規則";
			//
			// labelRule
			//
			this->labelRule->AutoSize = true;
			this->labelRule->Location = System::Drawing::Point(25, 145);
			this->labelRule->Name = L"labelRule";
			this->labelRule->Size = System::Drawing::Size(418, 48);
			this->labelRule->TabIndex = 3;
			this->labelRule->Text = L"遊戲由 9 個 3 × 3 個的九宮格組成。\r\n每一列的數字均須包含 1 ~ 9，不能缺少，也不能重複。\r\n每一行的數字均須包含 1 ~ 9，不能缺少，也不能重複"
				L"。\r\n每一宮 (粗黑線圍起來的區域) 的數字均須包含 1 ~ 9，不能缺少，也不能重複。\r\n";
			//
			// HelpForm
			//
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(619, 231);
			this->Controls->Add(this->labelRule);
			this->Controls->Add(this->labelRuleTitle);
			this->Controls->Add(this->labelIntroductionTitle);
			this->Controls->Add(this->labelIntroduction);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->MaximizeBox = false;
			this->Name = L"HelpForm";
			this->Text = L"規則說明";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &HelpForm::HelpForm_FormClosing);
			this->ResumeLayout(false);
			this->PerformLayout();
		}
#pragma endregion

	public:
		void applyFont() {
			System::Drawing::Font^ displayLargeTitle = ResourceHandler::getInstance()->getDisplayLargeTitleFont();
			System::Drawing::Font^ displayTitle = ResourceHandler::getInstance()->getDisplayTitleFont();
			labelIntroductionTitle->Font = displayLargeTitle;
			labelRuleTitle->Font = displayLargeTitle;

			labelIntroduction->Font = displayTitle;
			labelRule->Font = displayTitle;
		}
	private:
		System::Void HelpForm_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e) {
			Hide();
			e->Cancel = true;
		}
	};
}