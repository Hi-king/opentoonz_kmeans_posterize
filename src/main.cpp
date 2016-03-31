#define TNZU_DEFINE_INTERFACE
#include <toonz_utility.hpp>

class MyFx : public tnzu::Fx {
 public:
  //
  // PORT
  //
  enum {
    PORT_INPUT,
    PORT_COUNT,
  };
  int port_count() const override { return PORT_COUNT; }
  char const* port_name(int i) const override {
    static std::array<char const*, PORT_COUNT> names = {
        "Input",
    };
    return names[i];
  }

  //
  // PARAM GROUP
  //
  enum {
    PARAM_GROUP_DEFAULT,
    PARAM_GROUP_COUNT,
  };
  int param_group_count() const override { return PARAM_GROUP_COUNT; }
  char const* param_group_name(int i) const override {
    static std::array<char const*, PARAM_GROUP_COUNT> names = {
        "Default",
    };
    return names[i];
  }

  //
  // PARAM
  //
  enum {
    PARAM_K,
    PARAM_COUNT,
  };
  int param_count() const override { return PARAM_COUNT; }
  ParamPrototype const* param_prototype(int i) const override {
    static std::array<ParamPrototype, PARAM_COUNT> const params = {
        // name, group, default, min, max
        ParamPrototype{"k", PARAM_GROUP_DEFAULT, 10, 1, 1000},
    };
    return &params[i];
  }

 public:
  int compute(Config const& config, Params const& params, Args const& args,
              cv::Mat& retimg) override try {
    DEBUG_PRINT(__FUNCTION__);

    if (args.invalid(PORT_INPUT)) {
      return 0;
    }
    int const k = params.get<int>(PARAM_K);
    cv::TermCriteria criteria(cv::TermCriteria::MAX_ITER+cv::TermCriteria::EPS, 50, FLT_EPSILON);

    cv::Mat input_img, img_reshaped, img_reshaped_converted, labels, centroids;
    input_img = args.get(PORT_INPUT);
    img_reshaped = input_img.reshape(0, input_img.rows * input_img.cols);
    img_reshaped.convertTo(img_reshaped_converted, CV_32FC4);

    cv::kmeans(img_reshaped_converted, k, labels, criteria, 1, cv::KMEANS_PP_CENTERS, centroids);

    for (int y = 0; y < img_reshaped.cols; ++y) {
        for (int x = 0; x < img_reshaped.rows; ++x) {
            uint assignment = labels.at<uint>(y, x);
            int channels = img_reshaped.channels();
            for (int c = 0; c < channels; ++c) {
                img_reshaped_converted.at<cv::Vec4f>(y, x)[c] = centroids.at<float>(assignment, c);
            }
        }
    }
    img_reshaped_converted.convertTo(img_reshaped, img_reshaped.type());
    retimg = img_reshaped.reshape(0, input_img.rows);

    return 0;
  } catch (cv::Exception const& e) {
    DEBUG_PRINT(e.what());
  }
};

namespace tnzu {
PluginInfo const* plugin_info() {
  static PluginInfo const info(TNZU_PP_STR(PLUGIN_NAME),    // name
                               TNZU_PP_STR(PLUGIN_VENDOR),  // vendor
                               "",                          // note
                               "http://dwango.co.jp/");     // helpurl
  return &info;
}

Fx* make_fx() { return new MyFx(); }
}
