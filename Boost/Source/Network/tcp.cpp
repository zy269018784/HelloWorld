#include <boost/asio.hpp>
#include <iostream>

void connect_to_host_sync(const std::string& host, const std::string& port) {
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::asio::ip::tcp::socket socket(io_context);

    try {
        // 解析域名
        boost::asio::ip::basic_resolver_results<boost::asio::ip::tcp> endpoints = resolver.resolve(host, port);
        //auto endpoints = resolver.resolve(host, port);
        for (const auto& result : endpoints) 
        {
            std::cout << "IP: " << result.endpoint().address().to_string()
                << ", Port: " << result.endpoint().port() << "\n";
        }
        
        // 尝试连接每个解析结果，直到成功
        auto ret = boost::asio::connect(socket, endpoints);
       
       // socket.async_read_some();
        std::cout << "Connected to " << host << ":" << port << "\n";

        // 这里可以进行数据读写操作...

        boost::system::error_code socket_error;
        socket.wait(boost::asio::socket_base::wait_type::wait_read, socket_error);

        if (socket_error.value() != 0)
        {
            std::cout << "socket_error " << socket_error << std::endl;
        }
        int nAva = socket.available(socket_error);
        std::cout << "nAva " << nAva << std::endl;

        std::vector<char> buffer(nAva); // 使用动态数组作为缓冲区

        uint32_t data_length = 0;
        //		boost::asio::read(Handle, boost::asio::buffer(&data_length, sizeof(data_length)));
        socket.read_some(boost::asio::buffer(buffer), socket_error);
        std::cout << "data_length " << data_length << std::endl;
        data_length = ntohl(data_length); // 将网络字节序转换为主机字节序

        std::cout << "data_length " << data_length << std::endl;

        socket.close();
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

int TestTCP()
{
    connect_to_host_sync("u731440-925f-57bd72c4.nmb1.seetacloud.com", "8443");
   // connect_to_host_sync("https://u731440-925f-57bd72c4.nmb1.seetacloud.com", "8443");
    return 0;
}