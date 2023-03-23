
namespace HQChart.CSharp.Test
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.TextCode = new System.Windows.Forms.TextBox();
            this.BtnRun = new System.Windows.Forms.Button();
            this.listResult = new System.Windows.Forms.ListView();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.Log = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // TextCode
            // 
            this.TextCode.Location = new System.Drawing.Point(7, 36);
            this.TextCode.Multiline = true;
            this.TextCode.Name = "TextCode";
            this.TextCode.Size = new System.Drawing.Size(776, 302);
            this.TextCode.TabIndex = 0;
            // 
            // BtnRun
            // 
            this.BtnRun.Location = new System.Drawing.Point(800, 57);
            this.BtnRun.Name = "BtnRun";
            this.BtnRun.Size = new System.Drawing.Size(197, 72);
            this.BtnRun.TabIndex = 1;
            this.BtnRun.Text = " Run";
            this.BtnRun.UseVisualStyleBackColor = true;
            this.BtnRun.Click += new System.EventHandler(this.BtnRun_Click);
            // 
            // listResult
            // 
            this.listResult.HideSelection = false;
            this.listResult.Location = new System.Drawing.Point(6, 384);
            this.listResult.Name = "listResult";
            this.listResult.Size = new System.Drawing.Size(1017, 233);
            this.listResult.TabIndex = 11;
            this.listResult.UseCompatibleStateImageBehavior = false;
            this.listResult.View = System.Windows.Forms.View.Details;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(9, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(91, 13);
            this.label1.TabIndex = 12;
            this.label1.Text = "通达信指标脚本";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(9, 358);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(55, 13);
            this.label2.TabIndex = 13;
            this.label2.Text = "执行结果";
            // 
            // Log
            // 
            this.Log.AutoSize = true;
            this.Log.Location = new System.Drawing.Point(237, 358);
            this.Log.Name = "Log";
            this.Log.Size = new System.Drawing.Size(13, 13);
            this.Log.TabIndex = 14;
            this.Log.Text = "--";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(187, 358);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(34, 13);
            this.label3.TabIndex = 15;
            this.label3.Text = "日志:";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1029, 620);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.Log);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.listResult);
            this.Controls.Add(this.BtnRun);
            this.Controls.Add(this.TextCode);
            this.Name = "Form1";
            this.Text = "HQChart通达信指标引擎C#调用例子";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox TextCode;
        private System.Windows.Forms.Button BtnRun;
        private System.Windows.Forms.ListView listResult;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label Log;
        private System.Windows.Forms.Label label3;
    }
}

