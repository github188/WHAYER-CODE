#ifndef HY_NETWORK_T_H
#define HY_NETWORK_T_H
#pragma warning(disable:4267)
/************************************************************************/
/*   底层库函数：集成GPU、CPU版本的不同网络层函数以及功能模块函数       */
/************************************************************************/
#define NUM_OBJECT 3  //对象
#define NUM_CLASSES 4  //分类 
#define INSULATOR_NUM_OBJECT 3  //对象
#define INSULATOR_NUM_CLASSES 4  //分类 
#define DEFECT_NUM_OBJECT 4  //对象
#define DEFECT_NUM_CLASSES 5  //分类 
#define NUM_ANCHORS 25 //anchors 
#define NUM_ROIS 300

#define HY_Network_EXPORTS

#ifdef __cplusplus //指明函数的编译方式，以得到没有任何修饰的函数名
extern "C"
{
#endif

#ifdef HY_Network_EXPORTS
#define HY_Network_API __declspec(dllexport) //导出符号宏定义
#else
#define HY_Network_API __declspec(dllimport)
#endif

	//PVANET 检测
	class HY_Network_API HYPVA_Detect{

	public:
		void* PvaHandls;//GPU句柄变量，必须定义此变量 

		/*定义各层 weight、bias变量*/
		float *means3_pva;
		float* conv1_weights; float* conv1_bias;
		float* conv2_weights; float* conv2_bias;
		float* conv3_weights; float* conv3_bias;

		float* inc3a_conv1_weights; float* inc3a_conv1_bias;
		float* inc3a_conv3_1_weights; float* inc3a_conv3_1_bias;
		float* inc3a_conv3_2_weights; float* inc3a_conv3_2_bias;
		float* inc3a_conv5_1_weights; float* inc3a_conv5_1_bias;
		float* inc3a_conv5_2_weights; float* inc3a_conv5_2_bias;
		float* inc3a_conv5_3_weights; float* inc3a_conv5_3_bias;

		float* inc3b_conv1_weights; float* inc3b_conv1_bias;
		float* inc3b_conv3_1_weights; float* inc3b_conv3_1_bias;
		float* inc3b_conv3_2_weights; float* inc3b_conv3_2_bias;
		float* inc3b_conv5_1_weights; float* inc3b_conv5_1_bias;
		float* inc3b_conv5_2_weights; float* inc3b_conv5_2_bias;
		float* inc3b_conv5_3_weights; float* inc3b_conv5_3_bias;

		float* inc3c_conv1_weights; float* inc3c_conv1_bias;
		float* inc3c_conv3_1_weights; float* inc3c_conv3_1_bias;
		float* inc3c_conv3_2_weights; float* inc3c_conv3_2_bias;
		float* inc3c_conv5_1_weights; float* inc3c_conv5_1_bias;
		float* inc3c_conv5_2_weights; float* inc3c_conv5_2_bias;
		float* inc3c_conv5_3_weights; float* inc3c_conv5_3_bias;

		float* inc3d_conv1_weights; float* inc3d_conv1_bias;
		float* inc3d_conv3_1_weights; float* inc3d_conv3_1_bias;
		float* inc3d_conv3_2_weights; float* inc3d_conv3_2_bias;
		float* inc3d_conv5_1_weights; float* inc3d_conv5_1_bias;
		float* inc3d_conv5_2_weights; float* inc3d_conv5_2_bias;
		float* inc3d_conv5_3_weights; float* inc3d_conv5_3_bias;

		float* inc3e_conv1_weights; float* inc3e_conv1_bias;
		float* inc3e_conv3_1_weights; float* inc3e_conv3_1_bias;
		float* inc3e_conv3_2_weights; float* inc3e_conv3_2_bias;
		float* inc3e_conv5_1_weights; float* inc3e_conv5_1_bias;
		float* inc3e_conv5_2_weights; float* inc3e_conv5_2_bias;
		float* inc3e_conv5_3_weights; float* inc3e_conv5_3_bias;
		//-----------------------------------------------------------------------//
		float* inc4a_conv1_weights; float* inc4a_conv1_bias;
		float* inc4a_conv3_1_weights; float* inc4a_conv3_1_bias;
		float* inc4a_conv3_2_weights; float* inc4a_conv3_2_bias;
		float* inc4a_conv5_1_weights; float* inc4a_conv5_1_bias;
		float* inc4a_conv5_2_weights; float* inc4a_conv5_2_bias;
		float* inc4a_conv5_3_weights; float* inc4a_conv5_3_bias;

		float* inc4b_conv1_weights; float* inc4b_conv1_bias;
		float* inc4b_conv3_1_weights; float* inc4b_conv3_1_bias;
		float* inc4b_conv3_2_weights; float* inc4b_conv3_2_bias;
		float* inc4b_conv5_1_weights; float* inc4b_conv5_1_bias;
		float* inc4b_conv5_2_weights; float* inc4b_conv5_2_bias;
		float* inc4b_conv5_3_weights; float* inc4b_conv5_3_bias;

		float* inc4c_conv1_weights; float* inc4c_conv1_bias;
		float* inc4c_conv3_1_weights; float* inc4c_conv3_1_bias;
		float* inc4c_conv3_2_weights; float* inc4c_conv3_2_bias;
		float* inc4c_conv5_1_weights; float* inc4c_conv5_1_bias;
		float* inc4c_conv5_2_weights; float* inc4c_conv5_2_bias;
		float* inc4c_conv5_3_weights; float* inc4c_conv5_3_bias;

		float* inc4d_conv1_weights; float* inc4d_conv1_bias;
		float* inc4d_conv3_1_weights; float* inc4d_conv3_1_bias;
		float* inc4d_conv3_2_weights; float* inc4d_conv3_2_bias;
		float* inc4d_conv5_1_weights; float* inc4d_conv5_1_bias;
		float* inc4d_conv5_2_weights; float* inc4d_conv5_2_bias;
		float* inc4d_conv5_3_weights; float* inc4d_conv5_3_bias;

		float* inc4e_conv1_weights; float* inc4e_conv1_bias;
		float* inc4e_conv3_1_weights; float* inc4e_conv3_1_bias;
		float* inc4e_conv3_2_weights; float* inc4e_conv3_2_bias;
		float* inc4e_conv5_1_weights; float* inc4e_conv5_1_bias;
		float* inc4e_conv5_2_weights; float* inc4e_conv5_2_bias;
		float* inc4e_conv5_3_weights; float* inc4e_conv5_3_bias;

		float* upsample_weights; /*float* upsample_bias;*/
		float* convf_weights; float* convf_bias;
		float* rpn_conv1_weights; float* rpn_conv1_bias;
		float* rpn_bbox_pred_weights; float* rpn_bbox_pred_bias;
		float* rpn_cls_score_weights; float* rpn_cls_score_bias;
		float* anchors_base;

		float* fc6_L_weights; float* fc6_L_bias;
		float* fc6_U_weights; float* fc6_U_bias;
		float* fc7_L_weights; float* fc7_L_bias;
		float* fc7_U_weights; float* fc7_U_bias;
		float* cls_score1_weights; float* cls_score1_bias;
		float* bbox_pred1_weights; float* bbox_pred1_bias;

		HYPVA_Detect();
		~HYPVA_Detect();

		/*释放模型数据*/
		void ReleaseNetwork();


		/* object_detection PVANET检测函数*/
		/*
		*PvaHandles：句柄
		*imagdata：图片数据；im_height：图片高度；im_width：图片宽度；widstep：图片step信息
		*im_scale_x，im_scale_y：图片x,y缩放比例
		*num_keep：框子数目
		*dstcls_boxes：框子位置结果
		*dstcls_scores：框子得分结果（0.4以上）
		*/
		void Detection(void *PvaHandles, unsigned char* imagdata, int im_height, int im_width, int widstep, float im_scale_x, float im_scale_y, int* num_keep,int* dstcls_cls, float*& dstcls_boxes, float*& dstcls_scores);
		void InsulatorDetection(void *PvaHandles, unsigned char* imagdata, int im_height, int im_width, int widstep, float im_scale_x, float im_scale_y, int* num_keep, int* dstcls_cls, float*& dstcls_boxes, float*& dstcls_scores);
		void DefectDetection(void *PvaHandles, unsigned char* imagdata, int im_height, int im_width, int widstep, float im_scale_x, float im_scale_y, int* num_keep, int* dstcls_cls, float*& dstcls_boxes, float*& dstcls_scores);
		/*初始化PVANET的数据，即导入各层的weight、bias*/
		int CreatNetwork(const char* pvamodel_path);
		int InsulatorCreatNetwork(const char* pvamodel_path);
		int DefectCreatNetwork(const char* pvamodel_path);


	};

	
#ifdef __cplusplus
}
#endif

#endif




