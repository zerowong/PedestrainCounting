/**
 * This is a container for muiltiple targets.
 *
 * @author Zhengrong Wang
 */

#ifndef TARGETS_FREELIST_HEADER
#define TARGETS_FREELIST_HEADER

#include "SingleTarget.h"

struct TargetsFreeListNode {
	SingleTarget *target;
	TargetsFreeListNode *nextFree;
	bool isFree;

	TargetsFreeListNode(int numParticles, 
		int numSelectors, int numWeakClassifiers, int numBackups);
	~TargetsFreeListNode();
};

class TargetsFreeList {
public:
	TargetsFreeList(int capacity, int numParticles, int numSelectors, 
		int numWeakClassifiers, int numBackups, float detWeight);
	~TargetsFreeList();

	/**
	 * Reset one target.
	 */
	void ResetOneTarget(int index);

	int InitializeTarget(const Rect &target, const Point2D &initVelocity);

	/**
	 * Propagate all the targets' particles.
	 */
	void Propagate(const Size &imgSize);

	/**
	 * Observe all the targets' partices.
	 */
	void Observe(const IntegralImage *intImage, Pool<Rect> &detections);

	// void Update(int index, const IntegralImage *intImage, );

	/**
	 * Calculate the target-detection matching score matrix.
	 *
	 * @param intImage		integral image
	 * @param dets			detections
	 * @param matchMat		matching score matrix
	 */
	void CalculateMatchScore(const IntegralImage *intImage, 
		const Pool<Rect> &dets, Pool<Pool<float>> &matchMat) const;

	// The array of nodes.
	std::vector<TargetsFreeListNode> listNodes;
	const int capacity;

	// Weight for detection term in particle observation.
	const float detectionWeight;

	// Allow MatchMatrix to modify the matchDets array.
	friend class MatchMatrix;

private:

	TargetsFreeListNode *freeNodes;

	// The (detection, target) pair.
	// Set by MatchMatrix.
	Pool<int> matchDets;
};


#endif
