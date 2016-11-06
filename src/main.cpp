#include <iostream>
#include <future>
#include <boost/filesystem.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/imgcodecs.hpp>

namespace fs = boost::filesystem;

auto constexpr MAX_ARGS = 2UL;
auto constexpr MIN_ARGS = 1UL;
auto constexpr EXTENSION = ".png";
auto constexpr FILENAME_LENGTH = 6UL;


auto printHelp () -> void {
  std::cout << "Extract .pngs files from a video.\n";
  std::cout << "Usage:\n";
  std::cout << "ocv_extract_pngs video.avi output_dir/" << std::endl;
}

auto saveImage(cv::Mat const & image, std::string filename)  -> void {
  cv::imwrite (filename, image);
}


auto main (int argc, char ** argv) -> int {
  // TODO: load the video file.
  // load each frame and save to the specified video folder
  if ((argc > MAX_ARGS + 1) || (argc < MIN_ARGS + 1)) {
    std::cerr << "Incorrect number of arguments.\n";
    printHelp ();
    return -1;
  }

  auto output_dir = std::string {};

  if (argc == MIN_ARGS + 1) {
    auto input_video_file = std::string {argv[1]};
    auto input_video_file_base = input_video_file.substr (0, input_video_file.find_last_of ('.'));
    output_dir = input_video_file_base + "/";
  } else {
    output_dir = std::string {argv[2]};
  }

  try {
    auto capture = cv::VideoCapture (std::string{argv[1]});
    if (!capture.isOpened ()) {
      throw std::invalid_argument {"Video file cannot be opened."};
    }

    auto output_dir_path = fs::path {output_dir};

    if (!fs::exists (output_dir_path) || !fs::is_directory (output_dir_path)) {
      fs::create_directory (output_dir_path);
    }

    auto current_frame = cv::Mat {};
    auto frame_index = 1UL;

    std::vector <std::future <void>> futures;
    while (capture.read (current_frame)) {

      auto frame_index_string = std::to_string (frame_index);
      auto num_unfilled_chars = FILENAME_LENGTH - frame_index_string.size ();
      if (num_unfilled_chars > 0) {
        frame_index_string.insert (0, num_unfilled_chars, '0');
      }
      auto output_file = output_dir + frame_index_string + EXTENSION;
      std::cout << "Writing output file : " << output_file << std::endl;
//      futures.push_back (std::async (std::launch::async, saveImage, current_frame, output_file));
      cv::imwrite (output_file, current_frame);
      ++frame_index;
    }

    for (auto & future : futures) {
      future.get ();
    }
  }
  catch (std::exception const & ex) {
    std::cout << ex.what () << std::endl;
    return -1;
  }

}
