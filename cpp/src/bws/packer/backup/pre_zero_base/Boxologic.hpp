#pragma once

#include <vector>
#include <list>
#include <map>

#include <memory>

struct s_Instance
{
	double width;
	double height;
	double length;

	double layout_width;
	double layout_height;
	double layout_length;

	double volume;
};

struct Pallet
	: public s_Instance
{};

/**
 * �ڽ� ����.
 *
 * Packer������ Instance + Wrap �� �ش���
 */
struct BoxInfo
	: public s_Instance
{
	/**
	 * ���� �ڽ��� ȭ��ĭ �� ��ǥ.
	 *
	 * Wrap�� getX(), getY(), getZ() �� �ش�
	 */
	double cox, coy, coz;

	/**
	 * ���� �Ǿ��� �� ����
	 */
	bool is_packed;
};

struct Scrappad
{
	double cumx;
	double cumz;
};

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
		
		~Boxologic();

	private:
		void encode();

		void decode();

	private:
		/* -----------------------------------------------------------
			GETTERS
		----------------------------------------------------------- */
		auto fetch_scrap_min_z_left() -> std::list<struct Scrappad>::iterator;

		auto fetch_scrap_min_z_right() -> std::list<struct Scrappad>::iterator;

	public:
		/* -----------------------------------------------------------
			OPERATORS
		----------------------------------------------------------- */
		auto pack() -> std::pair<std::shared_ptr<Wrapper>, std::shared_ptr<InstanceArray>>;

	private:
		/* -----------------------------------------------------------
			3D_BIN_PACKER
		----------------------------------------------------------- */
		Pallet pallet;

		std::vector<struct BoxInfo> box_array;

		/**
		 * ���� dimension�� ���̾�.
		 *
		 * Ư�� ��ǰ�� �� ��ǰ�� Ư�� ���� ��������, ��� ��ǰ���� �׾Ƴ��� ������ ���̾��̴�.
		 *
		 * <ul>
		 *	<li>
		 *		���̾��� ũ�� (����).
		 *		��� �ڽ��� ������ �׾ƿ÷��� �� �ʿ��� �ּ� ũ���̴�.
		 *
		 *		��� Instance�� width, length, height�κ��� ���� ���� ���� ���� ������ ��� ����.
		 *	</li>
		 *	<li>
		 *		���� ũ��, unique key.
		 *		���� �࿡���� Ư�� �ڽ��� ũ��κ��� ���Ѵ�. ��, �ߺ��� �� ����.
		 *
		 *		Instance�� width, length, height �� �ϳ�. ��, �����ؾ� ��.
		 *	</li>
		 * </ul>
		 */
		std::map<double, double> layer_map;
		
		std::list<struct Scrappad> scrap_list;
		std::list<struct Scrappad>::iterator scrap_min_z;

		/**
		 * Packing�� ��� ������ ���� ���� flag ��.
		 * execute_iterations() �� ���̾� �� iteration �� packing �������� ����ȭ �ߴ� ���θ� ����ϱ� ���� flag�̴�.
		 *
		 * Target to be local.
		 */
		bool packing;
		bool layerdone;
		bool evened;

		/**
		 * ��ġ ���� ����, orientation.
		 *
		 * Target to be local.
		 */
		int variant;

		/**
		 * ���� packing case�� ��ġ ����.
		 */
		int best_variant;

		bool packing_best;
		bool hundred_percent;

		double boxx, boxy, boxz;
		double bboxx, bboxy, bboxz;
		size_t boxi, bboxi;

		/**
		 * ���� Ž�� ���� �ڽ��� ��ġ���� �� index (current_box_???).
		 */
		double cboxx, cboxy, cboxz;
		size_t cboxi;
		double bfx, bfy, bfz;
		double bbfx, bbfy, bbfz;

		/**
		 * ��ü �ڽ� ����.
		 *
		 * Wrapper::reserveds->size() �� �ش�.
		 */
		size_t total_boxes;

		double layerinlayer;
		double prelayer;
		double lilz;

		/**
		 * iteration�� ������ Ƚ��.
		 *
		 * �ϳ��� iteration�� �ϳ��� packing case�� �ǹ���.
		 */
		size_t number_of_iterations;

		/**
		 * ȭ��ĭ�� ���� ������ ũ��.
		 */
		double remainpx, remainpy, remainpz;
		double packedy;
		double prepackedy;
		double layer_thickness;
		double itelayer;
		double preremainpy;

		/**
		 * ���� packing case�� ����� layer�� index ��ȣ
		 */
		double best_iteration;

		size_t packed_boxes;

		/**
		 * ���� packing case���� ���� �ڽ� ����.
		 */
		size_t number_packed_boxes;

		double packed_volume;

		/**
		 * ���� packing case�� Ȱ�� ����.
		 */
		double best_solution_volume;
		
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
		int find_layer(double thickness);

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
		void find_box(double hmx, double hy, double hmy, double hz, double hmz);

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
		void analyze_box(size_t x, double hmx, double hy, double hmy, double hz, double hmz, double dim1, double dim2, double dim3);

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
		void checkfound(); //TODO: Find better name for this
		
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