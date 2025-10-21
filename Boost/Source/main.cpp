#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <iostream>

boost::asio::io_service io;
boost::asio::signal_set signals(io, SIGINT);

void worker_thread() {
    while (true) {
        std::cout << "Working..." << std::endl;
        boost::this_thread::sleep_for(boost::chrono::seconds(1));
    }

    std::cout << "work end" << std::endl;
}

void signal_handler(
    const boost::system::error_code& error,
    int /*signal_number*/)
{
    if (!error) {
        std::cout << "\nInterrupt received, stopping threads..." << std::endl;
        io.stop();
    }
}
int TestSignal()
{
    signals.async_wait(signal_handler);

    // 启动工作线程
    boost::thread worker(worker_thread);

    std::cout << "Press Ctrl+C to exit..." << std::endl;
    io.run();

    // 等待工作线程结束
    //worker.interrupt();
    worker.join();

    std::cout << "All threads stopped, exiting..." << std::endl;
    return 0;
}

int TestTCP();


int main() 
{
    return TestTCP();
}