#include <boost/program_options.hpp>

#include "hydra/hydra_main.h"
#include "hydra/hydra_uhd_interface.h"

using namespace boost::program_options;

// Instantiate the backend object
hydra::uhd_hydra_sptr backend;

void
signal_handler(int signum)
{
  std::cout << "Closing Application." << std::endl;
  backend->release();
}

void on_age(int age)
{
  std::cout << "On age: " << age << '\n';
}

int
main(int argc, const char *argv[])
{
   signal(SIGINT, signal_handler);
   signal(SIGHUP, signal_handler);
   // signal(SIGQUIT, signal_handler);
   // signal(SIGABRT, signal_handler);

  // Boost Program Options configuration
  try
  {
    options_description desc{"Options"};
    desc.add_options()
      ("help,h", "Help Message")
      ("tx_freq", value<double>()->default_value(2e9), "Transmitter Centre Frequency")
      ("tx_rate", value<double>()->default_value(1e6), "Transmitter Sampling Rate")
      ("tx_fft", value<unsigned int>()->default_value(1024), "Transmitter FFT Size")
      ("rx_freq", value<double>()->default_value(2e9), "Receiver Centre Frequency")
      ("rx_rate", value<double>()->default_value(1e6), "Receiver Sampling Rate")
      ("rx_fft", value<unsigned int>()->default_value(1024), "Receiver FFT Size")
      ("host", value<std::string>()->default_value("127.0.0.1"), "Server Host")
      ("port", value<unsigned int>()->default_value(5000), "Server Port")
      ("backend", value<std::string>()->default_value("usrp"), "Backend (usrp, loop, plot)");

    // Instantiate variables map, store, and notify methods
    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);

    // Print help and exit
    if (vm.count("help"))
    {
      std::cout << desc << '\n';
      return 0;
    }

    // Print debug information
    std::string delimiter = std::string(20, '-');
    std::cout << delimiter << " Transmitter Parameters " << delimiter << std::endl;
    std::cout << "Centre Frequency " << vm["tx_freq"].as<double>();
    std::cout << "\t"<< "Sampling Rate " << vm["tx_rate"].as<double>();
    std::cout << "\t"<< "FFT Size " << vm["tx_fft"].as<unsigned int>() << "\n" << std::endl;
    std::cout << delimiter << "  Receiver Parameters   " << delimiter << std::endl;
    std::cout << "Centre Frequency " << vm["rx_freq"].as<double>();
    std::cout << "\t"<< "Sampling Rate " << vm["rx_rate"].as<double>();
    std::cout << "\t"<< "FFT Size " << vm["rx_fft"].as<unsigned int>() << "\n" << std::endl;
    std::cout << delimiter << "    Server Parameters   " << delimiter << std::endl;
    std::cout << "Host " << vm["host"].as<std::string>();
    std::cout << "\t" << "Port " << vm["port"].as<unsigned int>() << "\n" << std::endl;


    // Extract the backend type
    std::string backend_type = vm["backend"].as<std::string>();

    /* Configure the backend */
    if (backend_type == "usrp")
    {
      backend = std::make_shared<hydra::device_uhd>();
      std::cout << "USRP Backend" <<std::endl;
    }
    else if (backend_type == "loop")
    {
      backend = std::make_shared<hydra::device_loopback>();
      std::cout << "Loopback Backend" <<std::endl;
    }
    else if (backend_type == "plot")
    {
      backend = std::make_shared<hydra::device_image_gen>();
      std::cout << "Plot Image Backend" <<std::endl;
    }
    else
    {
      // Throw error if the backend is not valid
      throw invalid_option_value("backend = " + backend_type);
    }

   /* TRANSMITTER */
   double d_tx_centre_freq = vm["tx_freq"].as<double>();
   double d_tx_samp_rate   = vm["tx_rate"].as<double>();
   unsigned int u_tx_fft_size = vm["tx_fft"].as<unsigned int>();

   /* RECEIVER */
   double d_rx_centre_freq = vm["rx_freq"].as<double>();
   double d_rx_samp_rate   = vm["rx_rate"].as<double>();
   unsigned int u_rx_fft_size = vm["rx_fft"].as<unsigned int>();

   /* Control port */
   unsigned int u_port = vm["port"].as<unsigned int>();
   std::string s_host = vm["host"].as<std::string>();


   /* Instantiate XVL */
   hydra::HydraMain main = hydra::HydraMain(s_host + ":" + std::to_string(u_port));

   main.set_tx_config(
     backend,
     d_tx_centre_freq,
     d_tx_samp_rate,
     u_tx_fft_size);

   main.set_rx_config(
     backend,
     d_rx_centre_freq,
     d_rx_samp_rate,
     u_rx_fft_size);

   /* Run server */
   main.run();
  }
  catch (const error &ex)
  {
    std::cerr << ex.what() << '\n';
  }

   /* Run server */
   return 0;
}
