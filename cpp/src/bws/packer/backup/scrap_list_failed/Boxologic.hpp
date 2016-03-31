#pragma once

#include <vector>
#include <list>
#include <map>

#include <memory>

struct BoxInfo;
struct Layer;
struct Scrappad;

namespace hiswill
{
namespace packer
{
	class Wrapper;
	class InstanceArray;

	class Boxologic
	{
	private:
		/**
		 *
		 */
		std::shared_ptr<Wrapper> wrapper;

		/**
		 *
		 */
		std::shared_ptr<InstanceArray> instanceArray;

		/**
		 *
		 */
		std::shared_ptr<InstanceArray> leftInstanceArray;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * Construct from a wrapper and instances.
		 *
		 * @param wrapper
		 * @param instanceArray
		 */
		Boxologic(std::shared_ptr<Wrapper>, std::shared_ptr<InstanceArray>);
		
		//virtual ~Boxologic() = default;

	private:
		void encode();

		void decode();

	public:
		/* -----------------------------------------------------------
			OPERATORS
		----------------------------------------------------------- */
		auto pack() -> std::pair<std::shared_ptr<Wrapper>, std::shared_ptr<InstanceArray>>;

	private:
		/* -----------------------------------------------------------
			3D_BIN_PACKER
		----------------------------------------------------------- */
		std::vector<struct BoxInfo> box_list;
		std::map<long int, long int> layer_list;

		std::list<struct Scrappad> scrap_list;
		std::list<struct Scrappad>::iterator scrap_min_z;

		/**
		 * Packing�� ��� ������ ���� ���� flag ��.
		 * execute_iterations() �� ���̾� �� iteration �� packing �������� ����ȭ �ߴ� ���θ� ����ϱ� ���� flag�̴�.
		 *
		 * Target to be local.
		 */
		bool packing;
		bool layer_done;
		bool evened;

		/**
		 * ��ġ ���� ����, orientation.
		 *
		 * Target to be local.
		 */
		char variant;

		/**
		 * ���� packing case�� ��ġ ����.
		 */
		char best_variant;

		bool best_packing;
		bool hundred_percent;

		short int boxx, boxy, boxz, boxi;
		short int bboxx, bboxy, bboxz, bboxi;

		/**
		 * ���� Ž�� ���� �ڽ��� ��ġ���� �� index (current_box_???).
		 */
		short int cboxx, cboxy, cboxz, cboxi;
		short int bfx, bfy, bfz;
		short int bbfx, bbfy, bbfz;

		/**
		 * ȭ��ĭ�� ũ�� ����.
		 *
		 * Wrapper�� width, length, height�� �ش���
		 */
		short int xx, yy, zz;

		/**
		 * ȭ��ĭ�� ���⺰ ũ�� ����.
		 *
		 * ȭ��ĭ�� orientation�� ���� �� �ִ� �� ������, �̿� ���Ͽ� �� �˾ƺ��� �Ѵ�.
		 */
		short int pallet_x, pallet_y, pallet_z;

		/**
		 * ��ü �ڽ� ����.
		 *
		 * Wrapper::reserveds->size() �� �ش�.
		 */
		short int total_boxes;

		/**
		 * index i ó�� ���δ�.
		 *
		 * Target to be local.
		 */
		short int x;
		
		short int layerinlayer;
		short int prelayer;
		short int lilz;

		/**
		 * ȭ��ĭ�� ���� ������ ũ��.
		 */
		short int remainpx, remainpy, remainpz;
		short int packedy;
		short int prepackedy;
		short int layerthickness;
		short int itelayer;
		short int preremainpy;

		/**
		 * ���� packing case�� ����� layer�� key (dim)
		 */
		long int best_layer;
		short int packednumbox;

		/**
		 * ���� packing case���� ���� �ڽ� ����.
		 */
		short int number_packed_boxes;

		double packedvolume;

		/**
		 * ���� packing case�� Ȱ�� ����.
		 */
		double best_solution_volume;

		/**
		 * ȭ��ĭ�� ����. xx x yy x zz).
		 *
		 * Wrapper�� getVolume() �� �ش�.
		 */
		double total_pallet_volume;

		/**
		 * ��ü �ڽ��� ���� ��.
		 *
		 * sum (Instance::getVolume()) �� �ش�.
		 */
		double total_box_volume;
		double temp;

		/**
		 * ������ packing case�� ���� Ȱ�뵵.
		 */
		double pallet_volume_used_percentage;
		double packed_box_percentage;

		/**
		 * Execute iterations by calling proper functions.
		 *
		 * Iterations are done and parameters of the best solution are found.
		 */
		void execute_iterations(); //TODO: Needs a better name yet

		/**
		 * Construct layers.
		 *
		 * Lists all possible layer heights by giving a weight value to each of them.
		 */
		void list_candidate_layers();

		/**
		 * Update the linked list and the Boxlist[] array as a box is packed.
		 *
		 * Packs the boxes found and arranges all variables and records properly.
		 */
		int pack_layer();

		/**
		 * Find the most proper layer height by looking at the unpacked boxes and 
		 * the remaining empty space available.
		 */
		int find_layer(short int thickness);

		/**
		 * Determine the gap with the samllest z value in the current layer.
		 *
		 * Find the most proper boxes by looking at all six possible orientations,
		 * empty space given, adjacent boxes, and pallet limits.
		 *
		 * @param hmx Maximum available x-dimension of the current gap to be filled.
		 * @param hy Current layer thickness value.
		 * @param hmy Current layer thickness value.
		 * @param hz Z-dimension of the current gap to be filled.
		 * @param hmz Maximum available z-dimension to the current gap to be filled.
		 */
		void find_box(short int hmx, short int hy, short int hmy, short int hz, short int hmz);

		/**
		 * Used by find_box to analyze box dimensions.
		 *
		 * Analyzes each unpacked box to find the best fitting one to the empty space.
		 *
		 * @param hmx Maximum available x-dimension of the current gap to be filled.
		 * @param hy Current layer thickness value.
		 * @param hmy Current layer thickness value.
		 * @param hz Z-dimension of the current gap to be filled.
		 * @param hmz Maximum available z-dimension to the current gap to be filled.
		 *
		 * @param dim1 X-dimension of the orientation of the box being examined.
		 * @param dim2 Y-dimension of the orientation of the box being examined.
		 * @param dim3 Z-dimension of the orientation of the box being examined.
		 */
		void analyze_box(short int hmx, short int hy, short int hmy, short int hz, short int hmz, short int dim1, short int dim2, short int dim3);

		/**
		 * Determine the gap with the smallest z value in the current layer.
		 *
		 * Find the first to be packed gap i9n the layer edge.
		 */
		void find_smallest_z();

		/**
		 * After finding each box, the candidate boxes and the condition of the layer
		 * are examined.
		 */
		void check_found(); //TODO: Find better name for this
		
		/**
		 * After packing of each box, 100% packing condition is checked.
		 */
		void volume_check();

		/**
		 * Using the parameters found, packs the best solution found and reports.
		 */
		void report_results();

		/**
		 * Transforms the found coordinate system to the one entered by the user and 
		 * write them to the report.
		 */
		void write_boxlist_file();
	};
};
};

/* -----------------------------------------------------------
STRUCTURES
----------------------------------------------------------- */
/**
* �ڽ� ����.
*
* Packer������ Instance + Wrap �� �ش���
*/
struct BoxInfo
{
	/**
	* ���� �Ǿ��� �� ����
	*/
	bool is_packed;

	/**
	* �ڽ� ������ ������ ����.
	*
	* Instance�� width, length, height�� �ش��Ѵ�.
	*/
	short int width, height, length;

	/**
	* ���� �ڽ��� ȭ��ĭ �� ��ǥ.
	*
	* Wrap�� getX(), getY(), getZ() �� �ش�
	*/
	short int cox, coy, coz;

	/**
	* ���� �ڽ��� ȭ��ĭ �� ������ ����.
	* ȭ��ĭ �� ��ġ ������ �ݿ��� ������ �����̴�.
	*
	* Wrap�� getWidth(), getLength(), getHeight() �� �ش�
	*/
	short int layout_width, layout_height, layout_length;

	/**
	* ����. dim1 x dim2 x dim3
	*/
	long int vol;
};

///**
// * ���� dimension�� ���̾�.
// *
// * Ư�� ��ǰ�� �� ��ǰ�� Ư�� ���� ��������, ��� ��ǰ���� �׾Ƴ��� ������ ���̾��̴�.
// */
//struct Layer
//{
//	/**
//	* ���̾��� ũ�� (����).
//	* ��� �ڽ��� ������ �׾ƿ÷��� �� �ʿ��� �ּ� ũ���̴�.
//	*
//	* ��� Instance�� width, length, height�κ��� ���� ���� ���� ���� ������ ��� ����.
//	*/
//	long int layereval;
//
//	/**
//	 * ���� ũ��, unique key.
//	 * ���� �࿡���� Ư�� �ڽ��� ũ��κ��� ���Ѵ�. ��, �ߺ��� �� ����.
//	 *
//	 * Instance�� width, length, height �� �ϳ�. ��, �����ؾ� ��.
//	 */
//	short int layerdim;
//} *layer_list;

struct Scrappad
{
	short int cumx;
	short int cumz;
};