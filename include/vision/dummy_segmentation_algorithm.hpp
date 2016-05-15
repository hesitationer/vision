#ifndef UNBALL_DUMMY_SEGMENTATION_ALGORITHM_H_
#define UNBALL_DUMMY_SEGMENTATION_ALGORITHM_H_

#include <vision/raw_image.hpp>
#include <vision/segmentation_algorithm.hpp>

class DummySegmentationAlgorithm : public SegmentationAlgorithm
{
  public:
    DummySegmentationAlgorithm();

    void run();
};

#endif // UNBALL_DUMMY_SEGMENTATION_ALGORITHM_H_
