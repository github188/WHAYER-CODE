#include <opencv2/opencv.hpp>
#include <iostream>
#include "HY_Network_t.h"

using namespace std;
#define PRINT 1
#define CELVE 1
cv::Mat HYCD_imReszie(cv::Mat pImg, float *im_scale_x, float *im_scale_y);
typedef struct
{
	int left;
	int top;
	int right;
	int bottom;
} CRECT, *PCRECT;
typedef struct dr_result
{
	int flag;
	CRECT    Target;///目标rectangle
	float dVal;
}HYCD_RESULT;
typedef struct
{
	HYCD_RESULT *pResult;
	int  lResultNum;
}HYCD_RESULT_LIST;

#define MAX_LINE_SIZE 1024
static void sort(int n, const float* x, int* indices);
static int nms(int numBoxes, float overlap, int*& pick, float*& dstcls_boxes_t, float*& dstcls_scores_t);
int PVAInsulator(cv::Mat input, float score, HYCD_RESULT_LIST  *resultlist);
int PVADefect(cv::Mat input, int x1, int y1, int x2, int y2, float score, HYCD_RESULT_LIST  *resultlist);
int PVA(int argc, char **argv);
int PVAseparate(int argc, char **argv);
int main(int argc, char **argv)
{
	//while (1)
	{
		//PVAseparate(argc, argv);
		main321(argc, argv);
	}
		system("pause");
	return 0;
}
int PVAseparate(int argc, char **argv)//测试用
{
	//float nmsthreshold = 0.3;//argv[4]
	float score = 0.6;//argv[5]
	char savePath[256] = "D:\\result.csv";//argv[3]
	char imagelist[256] = "D:\\list.txt";//argv[1]
	char modelType[256] = "M3";//argv[2]

	char name[256] = "jueyuanzi";

	FILE* fpcsv;
	fpcsv = fopen(savePath, "w");
	fprintf(fpcsv, "filename,name,score,xmin,ymin,xmax,ymax\n");
	fclose(fpcsv);

	int c;
	FILE* fp;
	int lines = 0;
	int linesflag = 0;
	char *one_line;
	one_line = (char*)malloc(MAX_LINE_SIZE * sizeof(char));
	if ((fp = fopen(imagelist, "r")) == NULL)
	{
		printf("can not open file %s!\n", imagelist);
		exit(-1);
	}
	while ((c = fgetc(fp)) != EOF)
	{
		if (linesflag == 0)
			linesflag = 1;
		if (c == '\n')
			lines++;
	}
	if (linesflag == 1)
		lines++;
	fseek(fp, 0L, SEEK_SET);
	int imgnum = 0;
	int PVAInsulatornum = 0;
	int PVADefectnum = 0;
	//while (fgets(one_line, MAX_LINE_SIZE, fp) != NULL)
	while (1)
	{
		imgnum++; 
		printf("imgnum=%d\n",imgnum);
		/**************
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		imgnum++;
		if (0 == strcmp(&(one_line[strlen(one_line) - 1]), "\n"))
		{
			one_line[strlen(one_line) - 1] = '\0';
		}
		_splitpath(one_line, NULL, NULL, fname, ext);

		printf("%d/%d %s%s\n", imgnum, lines, fname, ext);
		/**************/
		cv::Mat input = cv::imread("D:/jpg/1521708302705-jueyuanzi.JPG");
		//printf("%s\n",one_line);
		if (input.empty())
		{
			std::cout << " load  image error  !!!" << std::endl;
			continue;
			//system("pause");
			//exit(-1);
		}
		HYCD_RESULT_LIST  Insulatorlist = { 0 };
		Insulatorlist.pResult = (HYCD_RESULT*)malloc(100 * sizeof(HYCD_RESULT));
		PVAInsulator(input, score, &Insulatorlist);
		PVAInsulatornum++;
		printf("PVAInsulatornum=%d\n",PVAInsulatornum);
		HYCD_RESULT_LIST  Defectlist[100] = { 0 };
		for (int i = 0; i<100; i++)
		{
			Defectlist[i].pResult = (HYCD_RESULT*)malloc(100 * sizeof(HYCD_RESULT));
		}
		
		//if (Insulatorlist.lResultNum == 0)
		if (0)
		{
			Insulatorlist.lResultNum++;
			int x1 = 0.05*input.cols;
			int y1 = 0.05*input.rows;
			int x2 = input.cols - 0.05*input.cols;
			int y2 = input.rows - 0.05*input.rows;
			PVADefect(input, x1, y1, x2, y2, score, &Defectlist[0]);	
			PVADefectnum++;
			printf("PVADefectnum=%d\n", PVADefectnum);
		}
		else
		{
			for (int j = 0; j < Insulatorlist.lResultNum; j++)
			{
				int width = Insulatorlist.pResult[j].Target.right - Insulatorlist.pResult[j].Target.left;
				int height = Insulatorlist.pResult[j].Target.bottom - Insulatorlist.pResult[j].Target.top;
				int x1 = Insulatorlist.pResult[j].Target.left - 0.1*width;
				int y1 = Insulatorlist.pResult[j].Target.top - 0.1*height;
				int x2 = Insulatorlist.pResult[j].Target.right + 0.1*width;
				int y2 = Insulatorlist.pResult[j].Target.bottom + 0.1*height;
				if (x1 < 0)
					x1 = Insulatorlist.pResult[j].Target.left;
				if (y1 < 0)
					y1 = Insulatorlist.pResult[j].Target.top;
				if (x2 >= input.cols)
					x2 = Insulatorlist.pResult[j].Target.right;
				if (y2 >= input.rows)
					y2 = Insulatorlist.pResult[j].Target.bottom;
				PVADefect(input, x1, y1, x2, y2, score, &Defectlist[j]);	
				PVADefectnum++;
				printf("PVADefectnum=%d\n", PVADefectnum);
			}
		}
		for (int i = 0; i<100; i++)
		{
			free(Defectlist[i].pResult);

		}

		input.release();
		free(Insulatorlist.pResult);
	}
	printf("---process complete---");
	fclose(fp);
	free(one_line);
	return 0;
	/**************************/
}
int PVA(int argc, char **argv)
{
	float nmsthreshold = 0.3;//argv[4]
	float score = 0.6;//argv[5]
	char savePath[256] = "E:\\result321.csv";//argv[3]
	char imagelist[256] = "D:\\list3_backup.txt";//argv[1]
	char modelType[256] = "M3";//argv[2]

	//float nmsthreshold = atof(argv[4]);//argv[4]
	//float score = atof(argv[5]);//argv[5]
	//char *savePath = argv[3];//argv[3]
	//char *imagelist = argv[1];//argv[1]
	//char *modelType = argv[2];//argv[2]

	char name[256] = "jueyuanzi";
	
	int load_res=0;
	
	
	
	int Num_rois = 0;//框子数目
	float* dstcls_scores_t = (float*)malloc(100 * sizeof(float));//框子得分（0.4以上）
	float* dstcls_boxes_t = (float*)malloc(4 * 100 * sizeof(float));//框子位置
	int* dstcls_cls_t = (int*)malloc(100 * sizeof(int));//框子分类信息
	float im_scale_x;
	float im_scale_y;
	FILE* fpcsv;
	fpcsv = fopen(savePath, "w");
	fprintf(fpcsv, "filename,name,score,xmin,ymin,xmax,ymax\n");
	fclose(fpcsv);
	
	int c;
	FILE* fp;
	int lines = 0;
	int linesflag = 0;
	char *one_line;
	one_line = (char*)malloc(MAX_LINE_SIZE * sizeof(char));
	if ((fp = fopen(imagelist, "r")) == NULL)
	{
		printf("can not open file %s!\n", imagelist);
		exit(-1);
	}
	while ((c = fgetc(fp)) != EOF)
	{ 
		if (linesflag == 0)
			linesflag = 1;
		if (c == '\n') 
			lines++;
	}
	if (linesflag==1)
		lines++;
	fseek(fp, 0L, SEEK_SET);
	int imgnum = 0;
	HYPVA_Detector  Insulator;
	load_res = Insulator.InsulatorCreatNetwork("./jueyuanzi1.bin");//第三步：初始化网络模型
	if (load_res != 0) {
		std::cout << "model loading failed ... " << std::endl;
		
		return load_res;
	}
	HYPVA_Detector  Defect;
	load_res = Defect.DefectCreatNetwork("./jueyuanzi_xiao_4.bin");//第三步：初始化网络模型
	if (load_res != 0) {
		std::cout << "model loading failed ... " << std::endl;
		return load_res;
	}
	while (fgets(one_line, MAX_LINE_SIZE, fp) != NULL)
	{
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		imgnum++;
		if (0 == strcmp(&(one_line[strlen(one_line) - 1]), "\n"))
		{
			one_line[strlen(one_line) - 1] = '\0';
		}
		
		_splitpath(one_line, NULL, NULL, fname, ext);
		
		printf("%d/%d %s%s\n", imgnum, lines, fname, ext);
		
		cv::Mat input = cv::imread(one_line);
		
		if (input.empty())
		{
			std::cout << " load  image error  !!!" << std::endl;
			continue;
		}
		cv::Mat scale_im;
		scale_im = HYCD_imReszie(input, &im_scale_x, &im_scale_y);
		HYCD_RESULT_LIST  Insulatorlist = { 0 };
		Insulatorlist.pResult = (HYCD_RESULT*)malloc(100 * sizeof(HYCD_RESULT));
		Num_rois = 0;
		memset(dstcls_scores_t, 0, sizeof(float) * 100);
		memset(dstcls_boxes_t, 0, sizeof(float) * 100 * 4);
		memset(dstcls_cls_t, 0, sizeof(int) * 100);
		
		Insulator.InsulatorDetection(Insulator.PvaHandls, scale_im.data, scale_im.rows, scale_im.cols, (int)scale_im.step, im_scale_x, im_scale_y, &Num_rois, dstcls_cls_t, dstcls_boxes_t, dstcls_scores_t);
		
		/*****************************************
		int *Insulatorpick;
		nms(Num_rois, 0.5, Insulatorpick, dstcls_boxes_t, dstcls_scores_t);
		for (int i = 1; i <= Insulatorpick[0]; i++)
		{
			if (dstcls_scores_t[Insulatorpick[i]] > score)
			{
				float x = dstcls_boxes_t[Insulatorpick[i] * 4];//矩形的左上角x坐标
				float y = dstcls_boxes_t[Insulatorpick[i] * 4 + 1];//矩形的左上角y坐标
				float height = dstcls_boxes_t[Insulatorpick[i] * 4 + 3] - dstcls_boxes_t[Insulatorpick[i] * 4 + 1];//矩形的长
				float width = dstcls_boxes_t[Insulatorpick[i] * 4 + 2] - dstcls_boxes_t[Insulatorpick[i] * 4];//矩形的宽 
				printf("%f %f %f %f\n", x, y, width, height);
				Insulatorlist.pResult[Insulatorlist.lResultNum].Target.left = x;
				Insulatorlist.pResult[Insulatorlist.lResultNum].Target.top = y;
				Insulatorlist.pResult[Insulatorlist.lResultNum].Target.right = x + width;
				Insulatorlist.pResult[Insulatorlist.lResultNum].Target.bottom = y + height;
				Insulatorlist.pResult[Insulatorlist.lResultNum].dVal = dstcls_scores_t[Insulatorpick[i]];
				Insulatorlist.pResult[Insulatorlist.lResultNum].flag = dstcls_cls_t[Insulatorpick[i]];
				Insulatorlist.lResultNum++;
			}
		}
		/************************/
		/************************/
		for (int i = 0; i<Num_rois; i++)
		{
			if (dstcls_scores_t[i] > score)//画出置信度在0.75以上的框子
			{
				//printf("11111\n");
				float x = dstcls_boxes_t[i * 4];//矩形的左上角x坐标
				float y = dstcls_boxes_t[i * 4 + 1];//矩形的左上角y坐标
				float height = dstcls_boxes_t[i * 4 + 3] - dstcls_boxes_t[i * 4 + 1];//矩形的长
				float width = dstcls_boxes_t[i * 4 + 2] - dstcls_boxes_t[i * 4];//矩形的宽 
				Insulatorlist.pResult[Insulatorlist.lResultNum].Target.left = x;
				Insulatorlist.pResult[Insulatorlist.lResultNum].Target.top = y;
				Insulatorlist.pResult[Insulatorlist.lResultNum].Target.right = x + width;
				Insulatorlist.pResult[Insulatorlist.lResultNum].Target.bottom = y + height;
				Insulatorlist.pResult[Insulatorlist.lResultNum].dVal = dstcls_scores_t[i];
				Insulatorlist.pResult[Insulatorlist.lResultNum].flag = dstcls_cls_t[i];
				Insulatorlist.lResultNum++;
			}
		}
		/************************/
		
		/*******************************
		if (Insulatorlist.lResultNum > 3)
		{
			for (int j = 0; j < Insulatorlist.lResultNum - 1; j++)
			{
				for (int i = 0; i < Insulatorlist.lResultNum - 1 - j; i++)
				{
					if (Insulatorlist.pResult[i].dVal < Insulatorlist.pResult[i + 1].dVal)
					{
						float tmpf = Insulatorlist.pResult[i].dVal;
						Insulatorlist.pResult[i].dVal = Insulatorlist.pResult[i + 1].dVal;
						Insulatorlist.pResult[i + 1].dVal = tmpf;
						tmpf = Insulatorlist.pResult[i].flag;
						Insulatorlist.pResult[i].flag = Insulatorlist.pResult[i + 1].flag;
						Insulatorlist.pResult[i + 1].flag = tmpf;
						int tmpi = Insulatorlist.pResult[i].Target.left;
						Insulatorlist.pResult[i].Target.left = Insulatorlist.pResult[i + 1].Target.left;
						Insulatorlist.pResult[i + 1].Target.left = tmpi;
						tmpi = Insulatorlist.pResult[i].Target.top;
						Insulatorlist.pResult[i].Target.top = Insulatorlist.pResult[i + 1].Target.top;
						Insulatorlist.pResult[i + 1].Target.top = tmpi;
						tmpi = Insulatorlist.pResult[i].Target.right;
						Insulatorlist.pResult[i].Target.right = Insulatorlist.pResult[i + 1].Target.right;
						Insulatorlist.pResult[i + 1].Target.right = tmpi;
						tmpi = Insulatorlist.pResult[i].Target.bottom;
						Insulatorlist.pResult[i].Target.bottom = Insulatorlist.pResult[i + 1].Target.bottom;
						Insulatorlist.pResult[i + 1].Target.bottom = tmpi;
					}
				}
			}
			Insulatorlist.lResultNum = 3;
		}
		/****************************************************/
		HYCD_RESULT_LIST  Defectlist[100] = { 0 };
		for (int i = 0; i<100; i++)
		{
			Defectlist[i].pResult = (HYCD_RESULT*)malloc(100 * sizeof(HYCD_RESULT));
		}
		int zibaoflag = 0;
		int wuhuiflag = 0;
		int totalwuhuiflag = 0;
		int zibaoflagS[100] = { 0 };
		int wuhuiflagS[100] = { 0 };
		int totalwuhuiflagS[100] = { 0 };
		if (Insulatorlist.lResultNum == 0)
		{
			cv::Mat imageROI;
			int x1 = 0.05*input.cols;
			int y1 = 0.05*input.rows;
			int x2 = input.cols - 0.05*input.cols;
			int y2 = input.rows - 0.05*input.rows;
			int width = x2 - x1;
			int height = y2 - y1;
			imageROI = input(cv::Rect(x1, y1, width, height));
			cv::Mat scale_img;
			scale_img = HYCD_imReszie(imageROI, &im_scale_x, &im_scale_y);
			Insulatorlist.lResultNum++;
			Num_rois = 0;
			memset(dstcls_scores_t, 0, sizeof(float) * 100);
			memset(dstcls_boxes_t, 0, sizeof(float) * 100 * 4);
			memset(dstcls_cls_t, 0, sizeof(int) * 100);
			
			Defect.DefectDetection(Defect.PvaHandls, scale_img.data, scale_img.rows, scale_img.cols, (int)scale_img.step, im_scale_x, im_scale_y, &Num_rois, dstcls_cls_t, dstcls_boxes_t, dstcls_scores_t);
			
			int *pick;
			nms(Num_rois, nmsthreshold, pick, dstcls_boxes_t, dstcls_scores_t);
			for (int i = 1; i<=pick[0]; i++)
			{
				if (dstcls_scores_t[pick[i]] > score)
				{
					float x = dstcls_boxes_t[pick[i] * 4];//矩形的左上角x坐标
					float y = dstcls_boxes_t[pick[i] * 4 + 1];//矩形的左上角y坐标
					float height = dstcls_boxes_t[pick[i] * 4 + 3] - dstcls_boxes_t[pick[i] * 4 + 1];//矩形的长
					float width = dstcls_boxes_t[pick[i] * 4 + 2] - dstcls_boxes_t[pick[i] * 4];//矩形的宽 
					if ((dstcls_cls_t[pick[i]] == 2 || dstcls_cls_t[pick[i]] == 3) && dstcls_scores_t[pick[i]] < score+0.06)
						continue;
					Defectlist[0].pResult[Defectlist[0].lResultNum].Target.left = x;
					Defectlist[0].pResult[Defectlist[0].lResultNum].Target.top = y;
					Defectlist[0].pResult[Defectlist[0].lResultNum].Target.right = x + width;
					Defectlist[0].pResult[Defectlist[0].lResultNum].Target.bottom = y + height;
					Defectlist[0].pResult[Defectlist[0].lResultNum].Target.left += x1;
					Defectlist[0].pResult[Defectlist[0].lResultNum].Target.top += y1;
					Defectlist[0].pResult[Defectlist[0].lResultNum].Target.right += x1;
					Defectlist[0].pResult[Defectlist[0].lResultNum].Target.bottom += y1;
					Defectlist[0].pResult[Defectlist[0].lResultNum].dVal = dstcls_scores_t[pick[i]];
					Defectlist[0].pResult[Defectlist[0].lResultNum].flag = dstcls_cls_t[pick[i]];
					Defectlist[0].lResultNum++;
					if (dstcls_cls_t[pick[i]] == 1 && zibaoflag == 0)
						zibaoflag = 1;
					if (dstcls_cls_t[pick[i]] == 2 && wuhuiflag == 0)
						wuhuiflag = 1;
					if (dstcls_cls_t[pick[i]] == 3 && totalwuhuiflag == 0)
						totalwuhuiflag = 1;

					if (dstcls_cls_t[pick[i]] == 1 && zibaoflagS[0] == 0)
						zibaoflagS[0] = 1;
					if (dstcls_cls_t[pick[i]] == 2 && wuhuiflagS[0] == 0)
						wuhuiflagS[0] = 1;
					if (dstcls_cls_t[pick[i]] == 3 && totalwuhuiflagS[0] == 0)
						totalwuhuiflagS[0] = 1;
				}
			}
			free(pick);
			imageROI.release();
			scale_img.release();
		}
		else
		{
			for (int j = 0; j < Insulatorlist.lResultNum; j++)
			{
				
				cv::Mat imageROI;
				int width = Insulatorlist.pResult[j].Target.right - Insulatorlist.pResult[j].Target.left;
				int height = Insulatorlist.pResult[j].Target.bottom - Insulatorlist.pResult[j].Target.top;
				int x1 = Insulatorlist.pResult[j].Target.left - 0.1*width;
				int y1 = Insulatorlist.pResult[j].Target.top - 0.1*height;
				int x2 = Insulatorlist.pResult[j].Target.right + 0.1*width;
				int y2 = Insulatorlist.pResult[j].Target.bottom + 0.1*height;
				if (x1 < 0)
					x1 = Insulatorlist.pResult[j].Target.left;
				if (y1 < 0)
					y1 = Insulatorlist.pResult[j].Target.top;
				if (x2 >= input.cols)
					x2 = Insulatorlist.pResult[j].Target.right;
				if (y2 >= input.rows)
					y2 = Insulatorlist.pResult[j].Target.bottom;
				width = x2 - x1;
				height = y2 - y1;
				
				imageROI = input(cv::Rect(x1, y1, width, height));
				cv::Mat scale_img;
				scale_img = HYCD_imReszie(imageROI, &im_scale_x, &im_scale_y);
				Num_rois = 0;
				memset(dstcls_scores_t, 0, sizeof(float) * 100);
				memset(dstcls_boxes_t, 0, sizeof(float) * 100 * 4);
				memset(dstcls_cls_t, 0, sizeof(int) * 100);
				//cv::imwrite("./imageROI.jpg", imageROI);
				//cv::imwrite("./scale_img.jpg", scale_img);//看切取出的结果图
				Defect.DefectDetection(Defect.PvaHandls, scale_img.data, scale_img.rows, scale_img.cols, (int)scale_img.step, im_scale_x, im_scale_y, &Num_rois, dstcls_cls_t, dstcls_boxes_t, dstcls_scores_t);
				
				/****************************************/
				int *pick;
				nms(Num_rois, nmsthreshold, pick, dstcls_boxes_t, dstcls_scores_t);
				for (int i = 1; i<=pick[0]; i++)
				{
					if (dstcls_scores_t[pick[i]] > score)
					{

						float x = dstcls_boxes_t[pick[i] * 4];//矩形的左上角x坐标
						float y = dstcls_boxes_t[pick[i] * 4 + 1];//矩形的左上角y坐标
						float height = dstcls_boxes_t[pick[i] * 4 + 3] - dstcls_boxes_t[pick[i] * 4 + 1];//矩形的长
						float width = dstcls_boxes_t[pick[i] * 4 + 2] - dstcls_boxes_t[pick[i] * 4];//矩形的宽 
						if ((dstcls_cls_t[pick[i]] == 2 || dstcls_cls_t[pick[i]] == 3) && dstcls_scores_t[pick[i]] < score+0.06)
							continue;
						Defectlist[j].pResult[Defectlist[j].lResultNum].Target.left = x;
						Defectlist[j].pResult[Defectlist[j].lResultNum].Target.top = y;
						Defectlist[j].pResult[Defectlist[j].lResultNum].Target.right = x + width;
						Defectlist[j].pResult[Defectlist[j].lResultNum].Target.bottom = y + height;
						Defectlist[j].pResult[Defectlist[j].lResultNum].Target.left += x1;
						Defectlist[j].pResult[Defectlist[j].lResultNum].Target.top += y1;
						Defectlist[j].pResult[Defectlist[j].lResultNum].Target.right += x1;
						Defectlist[j].pResult[Defectlist[j].lResultNum].Target.bottom += y1;
						Defectlist[j].pResult[Defectlist[j].lResultNum].dVal = dstcls_scores_t[pick[i]];
						Defectlist[j].pResult[Defectlist[j].lResultNum].flag = dstcls_cls_t[pick[i]];
						Defectlist[j].lResultNum++;
						if (dstcls_cls_t[pick[i]] == 1 && zibaoflag == 0)
						{
							zibaoflag = 1;
						}
						if (dstcls_cls_t[pick[i]] == 2 && wuhuiflag == 0)
							wuhuiflag = 1;
						if (dstcls_cls_t[pick[i]] == 3 && totalwuhuiflag == 0)
							totalwuhuiflag = 1;

						if (dstcls_cls_t[pick[i]] == 1 && zibaoflagS[j] == 0)
							zibaoflagS[j] = 1;
						if (dstcls_cls_t[pick[i]] == 2 && wuhuiflagS[j] == 0)
							wuhuiflagS[j] = 1;
						if (dstcls_cls_t[pick[i]] == 3 && totalwuhuiflagS[j] == 0)
							totalwuhuiflagS[j] = 1;
					}
				}
				free(pick);
				/****************************************/
				if (zibaoflagS[j] == 1)
				{
					wuhuiflagS[j] = 0;
					totalwuhuiflagS[j] = 0;
				}
				else if (wuhuiflagS[j] == 1)
				{
					totalwuhuiflagS[j] = 0;
				}
				
				imageROI.release();
				scale_img.release();
			}
		}
		
		/*****************************/
#if PRINT
		cv::Mat resultImage;
		resultImage = input.clone();
#endif
		fpcsv = fopen(savePath, "at+");
		if (NULL == fpcsv)
		{
			printf("can not open file %s!\n", savePath);
			exit(-1);
		}
		int DefectType = 0;
		int zibaonum = 0;
		int Defectnum = 0;
		int Defectmaxnum = 4;
		if (zibaoflag == 1)
		{
			DefectType++;
			for (int j = 0; j < Insulatorlist.lResultNum; j++)
			{
				if (zibaoflagS[j] == 0)continue;
				if (zibaonum == 2)break;
				if (Defectnum == Defectmaxnum)break;
				for (int i = 0; i < Defectlist[j].lResultNum; i++)
				{
					if (zibaonum == 2)break;
					if (Defectnum == Defectmaxnum)break;
					if (Defectlist[j].pResult[i].flag == 1)
					{
						int width = Defectlist[j].pResult[i].Target.right - Defectlist[j].pResult[i].Target.left;
						int height = Defectlist[j].pResult[i].Target.bottom - Defectlist[j].pResult[i].Target.top;
						if (width*height < 1800)continue;
						zibaonum++;
						Defectnum++;
						fprintf(fpcsv, "%s%s,%s,%.12f,%d,%d,%d,%d\n", fname, ext, name, Defectlist[j].pResult[i].dVal, Defectlist[j].pResult[i].Target.left, Defectlist[j].pResult[i].Target.top, Defectlist[j].pResult[i].Target.right, Defectlist[j].pResult[i].Target.bottom);
#if CELVE
						char text[256] = { 0 };
						int x = Defectlist[j].pResult[i].Target.left;
						int y = Defectlist[j].pResult[i].Target.top;
						
						cv::rectangle(resultImage, cv::Rect(x, y, width, height), cv::Scalar(255, 0, 0), 4);
						sprintf(text, "zibao", Defectlist[j].pResult[i].flag);
						cv::putText(resultImage, text, cv::Point(x, y), CV_FONT_HERSHEY_TRIPLEX, 4, cv::Scalar(255, 0, 0));
#endif
					}
				}
			}
		}
		if (wuhuiflag == 1 && DefectType!=2)
		{
			DefectType++;
			for (int j = 0; j < Insulatorlist.lResultNum; j++)
			{
				if (wuhuiflagS[j] == 0)continue;
				if (Defectnum == Defectmaxnum)break;
				for (int i = 0; i < Defectlist[j].lResultNum; i++)
				{
					if (Defectnum == Defectmaxnum)break;
					if (Defectlist[j].pResult[i].flag == 2)
					{
						int width = Defectlist[j].pResult[i].Target.right - Defectlist[j].pResult[i].Target.left;
						int height = Defectlist[j].pResult[i].Target.bottom - Defectlist[j].pResult[i].Target.top;
						if (width*height < 1800)continue;
						Defectnum++;
						fprintf(fpcsv, "%s%s,%s,%.12f,%d,%d,%d,%d\n", fname, ext, name, Defectlist[j].pResult[i].dVal, Defectlist[j].pResult[i].Target.left, Defectlist[j].pResult[i].Target.top, Defectlist[j].pResult[i].Target.right, Defectlist[j].pResult[i].Target.bottom);
#if CELVE
						char text[256] = { 0 };
						int x = Defectlist[j].pResult[i].Target.left;
						int y = Defectlist[j].pResult[i].Target.top;
						
						cv::rectangle(resultImage, cv::Rect(x, y, width, height), cv::Scalar(0, 255, 0), 4);
						sprintf(text, "wuhui", Defectlist[j].pResult[i].flag);
						cv::putText(resultImage, text, cv::Point(x, y), CV_FONT_HERSHEY_TRIPLEX, 4, cv::Scalar(0, 255, 0));
#endif
					}
				}
			}
		}
		if (totalwuhuiflag == 1 && DefectType != 2)
		{
			DefectType++;
			for (int j = 0; j < Insulatorlist.lResultNum; j++)
			{
				if (totalwuhuiflagS[j] == 0)continue;
				if (Defectnum == Defectmaxnum)break;
				if (zibaonum == 1 && Defectnum == 3)break;
				for (int i = 0; i < Defectlist[j].lResultNum; i++)
				{
					if (Defectnum == Defectmaxnum)break;
					if (zibaonum == 1 && Defectnum == 3)break;
					if (Defectlist[j].pResult[i].flag == 3)
					{
						int width = Defectlist[j].pResult[i].Target.right - Defectlist[j].pResult[i].Target.left;
						int height = Defectlist[j].pResult[i].Target.bottom - Defectlist[j].pResult[i].Target.top;
						if (width*height < 1800)continue;
						Defectnum++;
						fprintf(fpcsv, "%s%s,%s,%.12f,%d,%d,%d,%d\n", fname, ext, name, Defectlist[j].pResult[i].dVal, Defectlist[j].pResult[i].Target.left, Defectlist[j].pResult[i].Target.top, Defectlist[j].pResult[i].Target.right, Defectlist[j].pResult[i].Target.bottom);
#if CELVE
						char text[256] = { 0 };
						int x = Defectlist[j].pResult[i].Target.left;
						int y = Defectlist[j].pResult[i].Target.top;
						cv::rectangle(resultImage, cv::Rect(x, y, width, height), cv::Scalar(0, 0, 255), 4);
						sprintf(text, "totalwuhui");
						cv::putText(resultImage, text, cv::Point(x, y), CV_FONT_HERSHEY_TRIPLEX, 4, cv::Scalar(0, 0, 255));
#endif
						break;//一个绝缘子只画一个totalwuhui
					}
				}
			}
		}
		fclose(fpcsv);
#if PRINT && !CELVE
		for (int j = 0; j < Insulatorlist.lResultNum; j++)
		{
			for (int i = 0; i < Defectlist[j].lResultNum; i++)
			{
				char text[256] = { 0 };
				if (Defectlist[j].pResult[i].flag == 1)
					sprintf(text, "zibao");
				else if (Defectlist[j].pResult[i].flag == 2)
					sprintf(text, "wuhui");
				else if (Defectlist[j].pResult[i].flag == 3)
					sprintf(text, "totalwuhui");
				else
					continue;
				int x = Defectlist[j].pResult[i].Target.left;
				int y = Defectlist[j].pResult[i].Target.top;
				int width = Defectlist[j].pResult[i].Target.right - Defectlist[j].pResult[i].Target.left;
				int height = Defectlist[j].pResult[i].Target.bottom - Defectlist[j].pResult[i].Target.top;
				cv::rectangle(resultImage, cv::Rect(x, y, width, height), cv::Scalar(255, 0, 0), 4);//蓝色
				cv::putText(resultImage, text, cv::Point(x, y), CV_FONT_HERSHEY_TRIPLEX, 4, cv::Scalar(0, 255, 255));
			}
		}
#endif
#if PRINT
		char outputFile[100];
		sprintf(outputFile, "Insulator_result\\PtruckResult_%d.jpg", imgnum);
		cv::imwrite(outputFile, resultImage);
		resultImage.release();
#endif
		/************************************************/
		for (int i = 0; i<100; i++)
		{
			free(Defectlist[i].pResult);

		}
		
		input.release();
		scale_im.release();
		free(Insulatorlist.pResult);
	}
	printf("---process complete---\n");
	fclose(fp);
	free(one_line);
	free(dstcls_scores_t);
	free(dstcls_boxes_t);
	free(dstcls_cls_t);
	Insulator.ReleaseNetwork();
	Defect.ReleaseNetwork();
	//system("pause");
	return 0;
	/**************************/
}
int PVAInsulator(cv::Mat input,float score,HYCD_RESULT_LIST  *resultlist)
{
	HYPVA_Detector  Insulator;
	int load_res = Insulator.InsulatorCreatNetwork("../model/jueyuanzi1_1.bin");//第三步：初始化网络模型
	if (load_res != 0) {
		std::cout << "model loading failed ... " << std::endl;
		//system("pause");
		return load_res;
	}
	float im_scale_x;
	float im_scale_y;
	int Num_rois=0;
	float* dstcls_scores_t = (float*)malloc(100 * sizeof(float));//框子得分（0.4以上）
	float* dstcls_boxes_t = (float*)malloc(4 * 100 * sizeof(float));//框子位置
	int* dstcls_cls_t = (int*)malloc(100 * sizeof(float));//框子分类信息
	cv::Mat scale_im;
	scale_im = HYCD_imReszie(input, &im_scale_x, &im_scale_y);
	Insulator.InsulatorDetection(Insulator.PvaHandls, scale_im.data, scale_im.rows, scale_im.cols, (int)scale_im.step, im_scale_x, im_scale_y, &Num_rois, dstcls_cls_t, dstcls_boxes_t, dstcls_scores_t);
	for (int i = 0; i<Num_rois; i++)
	{
		if (dstcls_scores_t[i] > score)//画出置信度在0.75以上的框子
		{
			//printf("11111\n");
			float x = dstcls_boxes_t[i * 4];//矩形的左上角x坐标
			float y = dstcls_boxes_t[i * 4 + 1];//矩形的左上角y坐标
			float height = dstcls_boxes_t[i * 4 + 3] - dstcls_boxes_t[i * 4 + 1];//矩形的长
			float width = dstcls_boxes_t[i * 4 + 2] - dstcls_boxes_t[i * 4];//矩形的宽 

			resultlist->pResult[resultlist->lResultNum].Target.left = x;
			resultlist->pResult[resultlist->lResultNum].Target.top = y;
			resultlist->pResult[resultlist->lResultNum].Target.right = x + width;
			resultlist->pResult[resultlist->lResultNum].Target.bottom = y + height;
			resultlist->pResult[resultlist->lResultNum].dVal = dstcls_scores_t[i];
			resultlist->pResult[resultlist->lResultNum].flag = dstcls_cls_t[i];
			resultlist->lResultNum++;
		}
	}
	Insulator.ReleaseNetwork();
	scale_im.release();
	free(dstcls_cls_t), dstcls_cls_t = NULL;
	free(dstcls_scores_t), dstcls_scores_t = NULL;
	free(dstcls_boxes_t), dstcls_boxes_t = NULL;
}
int PVADefect(cv::Mat input, int x1, int y1, int x2, int y2, float score, HYCD_RESULT_LIST  *resultlist)
{
	HYPVA_Detector  Defect;
	int load_res = Defect.DefectCreatNetwork("../model/jueyuanzi_xiao.bin");//第三步：初始化网络模型
	if (load_res != 0) {
		std::cout << "model loading failed ... " << std::endl;
		//system("pause");
		return load_res;
	}
	float im_scale_x;
	float im_scale_y;
	int Num_rois=0;
	float* dstcls_scores_t = (float*)malloc(100 * sizeof(float));//框子得分（0.4以上）
	float* dstcls_boxes_t = (float*)malloc(4 * 100 * sizeof(float));//框子位置
	int* dstcls_cls_t = (int*)malloc(100 * sizeof(float));//框子分类信息
	cv::Mat imageROI;
	int width = x2 - x1;
	int height = y2 - y1;
	imageROI = input(cv::Rect(x1, y1, width, height));
	cv::Mat scale_img;
	scale_img = HYCD_imReszie(imageROI, &im_scale_x, &im_scale_y);
	Defect.DefectDetection(Defect.PvaHandls, scale_img.data, scale_img.rows, scale_img.cols, (int)scale_img.step, im_scale_x, im_scale_y, &Num_rois, dstcls_cls_t, dstcls_boxes_t, dstcls_scores_t);
	for (int i = 0; i <= Num_rois; i++)
	{
		if (dstcls_scores_t[i] > score)//画出置信度在score以上的框子
		{
			float x = dstcls_boxes_t[i * 4];//矩形的左上角x坐标
			float y = dstcls_boxes_t[i * 4 + 1];//矩形的左上角y坐标
			float height = dstcls_boxes_t[i * 4 + 3] - dstcls_boxes_t[i * 4 + 1];//矩形的长
			float width = dstcls_boxes_t[i * 4 + 2] - dstcls_boxes_t[i * 4];//矩形的宽 

			resultlist->pResult[resultlist->lResultNum].Target.left = x;
			resultlist->pResult[resultlist->lResultNum].Target.top = y;
			resultlist->pResult[resultlist->lResultNum].Target.right = x + width;
			resultlist->pResult[resultlist->lResultNum].Target.bottom = y + height;
			resultlist->pResult[resultlist->lResultNum].Target.left += x1;
			resultlist->pResult[resultlist->lResultNum].Target.top += y1;
			resultlist->pResult[resultlist->lResultNum].Target.right += x1;
			resultlist->pResult[resultlist->lResultNum].Target.bottom += y1;
			resultlist->pResult[resultlist->lResultNum].dVal = dstcls_scores_t[i];
			resultlist->pResult[resultlist->lResultNum].flag = dstcls_cls_t[i];
			resultlist->lResultNum++;
		}

	}
	Defect.ReleaseNetwork();
	imageROI.release();
	scale_img.release();
	free(dstcls_cls_t), dstcls_cls_t = NULL;
	free(dstcls_scores_t), dstcls_scores_t = NULL;
	free(dstcls_boxes_t), dstcls_boxes_t = NULL;
}
cv::Mat HYCD_imReszie(cv::Mat pImg, float *im_scale_x, float *im_scale_y)
{
	cv::Mat dst;
	int s_min = min(pImg.cols, pImg.rows);//需要对图片进行缩小，提高速度与准确度（该值为经验值）300
	int s_max = max(pImg.cols, pImg.rows);// 900
	int size_align = 32;
	int im_height = pImg.rows;
	int im_width = pImg.cols;
	int im_size_min = min(im_height, im_width);
	int im_size_max = max(im_height, im_width);
	float im_scale = 1.0f;
	if (s_max > 6000)
	{
		s_max = 6000;
		if ((int)floorf(im_scale * im_size_max + 0.5f) > s_max) {
			im_scale = s_max / (float)im_size_max;
		}
	}
	if (s_min <= 192)
	{
		s_min = 256;
		if ((int)floorf(im_scale * im_size_min + 0.5f) < s_min) {
			im_scale = s_min / (float)im_size_min;
		}
	}
	
	*im_scale_x = floor(im_width * im_scale / size_align) * size_align / im_width;
	*im_scale_y = floor(im_height * im_scale / size_align) * size_align / im_height;
	cv::resize(pImg, dst, cv::Size(0, 0), *im_scale_x, *im_scale_y, 1);
	return dst;
}
static int nms(int numBoxes, float overlap, int*& pick, float*& dstcls_boxes_t, float*& dstcls_scores_t)
{
	int i, j, index;
	int numBoxesOut = 0;//输出窗口数目
	float* box_area = (float*)malloc(numBoxes * sizeof(float));   // 定义窗口面积变量并分配空间 
	int* indices = (int*)malloc(numBoxes * sizeof(int));          // 定义窗口索引并分配空间 
	int* is_suppressed = (int*)malloc(numBoxes * sizeof(int));    // 定义是否抑制表标志并分配空间 
	// 初始化indices、is_supperssed、box_area信息 
	for (i = 0; i < numBoxes; i++)
	{
		indices[i] = i;
		is_suppressed[i] = 0;
		box_area[i] = (float)((dstcls_boxes_t[i * 4 + 2] - dstcls_boxes_t[i * 4] + 1) * (dstcls_boxes_t[i * 4 + 3] - dstcls_boxes_t[i * 4 + 1] + 1));
	}
	// 对输入窗口按照分数比值进行排序，排序后的编号放在indices中 
	sort(numBoxes, dstcls_scores_t, indices);
	for (i = 0; i < numBoxes; i++)                // 循环所有窗口 
	{
		if (!is_suppressed[indices[i]])           // 判断窗口是否被抑制 
		{
			for (j = i + 1; j < numBoxes; j++)    // 循环当前窗口之后的窗口 
			{
				if (!is_suppressed[indices[j]])   // 判断窗口是否被抑制 
				{
					float x1max = max(dstcls_boxes_t[indices[i] * 4], dstcls_boxes_t[indices[j] * 4]);                    // 求两个窗口左上角x坐标最大值 
					float x2min = min(dstcls_boxes_t[indices[i] * 4 + 2], dstcls_boxes_t[indices[j] * 4 + 2]);     // 求两个窗口右下角x坐标最小值 
					float y1max = max(dstcls_boxes_t[indices[i] * 4 + 1], dstcls_boxes_t[indices[j] * 4 + 1]);                     // 求两个窗口左上角y坐标最大值 
					float y2min = min(dstcls_boxes_t[indices[i] * 4 + 3], dstcls_boxes_t[indices[j] * 4 + 3]);     // 求两个窗口右下角y坐标最小值 
					float overlapWidth = max(0.0f, (x2min - x1max + 1));            // 计算两矩形重叠的宽度 
					float overlapHeight = max(0.0f, (y2min - y1max + 1));           // 计算两矩形重叠的高度 
					if (overlapWidth > 0 && overlapHeight > 0)
					{
						//float overlapPart = (overlapWidth * overlapHeight) / box_area[indices[j]];    // 计算重叠的比率

						float inter = overlapWidth * overlapHeight;
						float overlapPart = inter / (box_area[indices[i]] + box_area[indices[j]] - inter);
						if (overlapPart > overlap)          // 判断重叠比率是否超过重叠阈值 
						{
							is_suppressed[indices[j]] = 1;           // 将窗口j标记为抑制 
						}
					}
				}
			}
		}
	}

	// 初始化输出窗口数目0 
	for (i = 0; i < numBoxes; i++)
	{
		if (!is_suppressed[i]) (numBoxesOut)++;    // 统计输出窗口数目 
	}

	pick = (int*)malloc((numBoxesOut + 1) * sizeof(int));          // 分配输出的序号数组空间

	pick[0] = numBoxesOut;
	index = 1;
	for (i = 0; i < numBoxes; i++)                  // 遍历所有输入窗口 
	{
		if (!is_suppressed[indices[i]])             // 将未发生抑制的窗口信息保存到输出信息中 
		{
			pick[index] = indices[i];
			index++;
		}
	}

	free(indices);          // 释放indices空间 
	free(box_area);         // 释放box_area空间 
	free(is_suppressed);    // 释放is_suppressed空间 

	return 0;
}

// 排序函数，排序后进行交换的是indices中的数据
// n：排序总数
// x：带排序数
// indices：初始为0~n-1数目 
static void sort(int n, const float* x, int* indices)
{
	int i, j;
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++)
		{
			if (x[indices[j]] > x[indices[i]])
			{
				//float x_tmp = x[i];
				int index_tmp = indices[i];
				//x[i] = x[j];
				indices[i] = indices[j];
				//x[j] = x_tmp;
				indices[j] = index_tmp;
			}
		}
}




