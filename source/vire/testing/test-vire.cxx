//! \file  vire/testing/test_vire.cxx
//! \brief Vire library
//
// Copyright (c) 2015 by François Mauger <mauger@lpccaen.in2p3.fr>
//
// This file is part of Vire.
//
// Vire is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Vire is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire. If not, see <http://www.gnu.org/licenses/>.

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <vector>

// Third party:
// - Boost:
#include <boost/thread.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

// This project:
#include <vire/vire.h>
#include <vire/version.h>
#include <vire/resource_files.h>

void test_boost_threads();
void test_boost_thread_group();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'Vire' library."
              << std::endl;

    std::clog << "Vire version : "
              << vire::version::get_version() << std::endl;
    std::clog << "Vire resource dir : '"
              << vire::get_resource_files_dir(true) << "'" << std::endl;
    std::clog << "Vire resource file : '"
              << vire::get_resource_file("README.rst", true) << "'"
              << std::endl;

    // test_boost_threads();

    // test_boost_thread_group();

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

class callable
{
public:
  callable(int id_) : _id_(id_) {}
  void operator()();
private:
  int _id_;
};

static boost::mutex & clog_mutex()
{
  static boost::scoped_ptr<boost::mutex> _cm(new boost::mutex);
  return *_cm;
}

void print_hello_world(int i_)
{
  clog_mutex().lock();
  std::clog << "thread #" << i_ << ": Hello, World!\n";
  clog_mutex().unlock();
  return;
}

void callable::operator()()
{
  print_hello_world(_id_);
  return;
}

void test_boost_thread_group()
{
  std::clog << "\ntest_boost_thread_group: " << std::endl;
  int nthreads = 50;
  boost::thread_group threads;

  for (int i = 0; i < nthreads; i++) {
    callable hello(i);
    boost::thread * t = threads.create_thread(hello);
  }
  threads.join_all();
  return;
}

void test_boost_threads()
{
  std::clog << "\ntest_boost_threads: " << std::endl;
  int nthreads = 50;
  std::vector<boost::shared_ptr<boost::thread> > threads;
  threads.reserve(nthreads);

  for (int i = 0; i < nthreads; i++) {
    callable hello(i);
    threads.push_back(boost::make_shared<boost::thread>(hello));
  }

  for (int i = 0; i < (int) threads.size(); i++) {
    threads[i]->join();
  }
  return;
}
