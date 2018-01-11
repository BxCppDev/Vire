// test-cmsserver-agenda.cxx

// Ourselves:
#include <vire/cmsserver/agenda.h>

// Standard library:
#include <unistd.h>
#include <chrono>
#include <thread>

// This project:
#include <vire/vire.h>

void test_agenda_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmsserver::agenda' class."
              << std::endl;

    test_agenda_1();

    std::clog << "The end." << std::endl;
  } catch (std::exception & x) {
    std::cerr << "error: " << x.what() << std::endl;
    error_code = EXIT_FAILURE;
  } catch (...) {
    std::cerr << "error: " << "unexpected error !" << std::endl;
    error_code = EXIT_FAILURE;
  }
  vire::terminate();
  return (error_code);
}

void test_wait(vire::cmsserver::agenda * ag_, const std::size_t n_)
{
  std::clog << "test_wait: Main loops..." << std::endl;
  for (int i = 0; i < n_; i++) {
    std::clog << "test_wait: New loop..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::clog << "test_wait: End of loop." << std::endl;
  }
  std::clog << "test_wait: Stopping the agenda..." << std::endl;
  ag_->stop();
  std::clog << "test_wait: End of main loop." << std::endl;
  return;
}

void test_agenda_1()
{
  std::clog << "\ntest_agenda_1: basics" << std::endl;

  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(1,3);

  vire::resource::manager resourceMgr;
  vire::cmsserver::agenda ag;
  ag.set_name("agenda");
  ag.set_display_name("Agenda");
  ag.set_terse_description("The CMS server agenda");
  ag.set_logging_priority(datatools::logger::PRIO_DEBUG);
  ag.set_resources(resourceMgr);
  ag.set_reservations_store("./reservations.xml");
  ag.set_stop_file("~/agenda.stop");
  ag.set_purge_store(3);
  ag.initialize_simple();
  ag.tree_dump(std::clog, "Agenda: ");
  {
    std::thread main_thread(test_wait, &ag, 10);
    ag.start();
    sleep(1);

    for (int i = 0; i < 3; i++) {
      int sleep_time = distribution(generator);
      sleep(sleep_time);
      std::clog << "\ntest_agenda_1: Add a new reservation..." << std::endl;

      vire::cmsserver::session_info sinfo;
      sinfo.set_id(ag.get_next_reservation_id());
      sinfo.set_key("toto");
      sinfo.set_description("Toto");
      sinfo.set_role_id(1003);
      boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
      boost::posix_time::time_period when(now, now + boost::posix_time::hours(2));
      sinfo.set_when(when);
      sinfo.set_usecase_id("toto-server");
      sinfo.initialize_simple();

      vire::cmsserver::process_info pinfo;
      pinfo.set_name("Survey");
      pinfo.set_description("Data quality survey process");
      pinfo.set_model_id("snemo::demonstrator::cms::qsurvey_process");
      pinfo.set_config_urn("snemo:demonstrator:qsurvey-3.7");

      vire::cmsserver::session_reservation r;
      r.set_sinfo(sinfo);
      r.set_pinfo(pinfo);

      int32_t id = ag.add_reservation(r);
    }

    main_thread.join();
  }
  sleep(3); // Safety delay to enable termination of the agenda's thread
  ag.reset();
  std::clog << std::endl;
  return;
}
