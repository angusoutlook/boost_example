

struct talk_to_svr{
    talk_to_svr(const std::string* username):sock_(service),started_(true),username_
    void connect(ip::tcp::endpoint ep){
        sock_.connect(ep);
    }

    void loop(){
        write("login "+username_+"\n");
        read_answer();
        while(started_){
            write_request();
            read_answer()
            boost::this_thread::sleep(millisec(rand()%70000));
        }
    }

    std::string username() const { return username_;}

private:
    ip::tcp::socket sock_;
    enum    { max_msg = 1024;}
    int     already_read_;
    char    buff_[max_msg];
    bool    started_;
    std::string username_;

    void write_request(){
        write("ping\n");
    }
    void read_answer(){
        already_read_ =0;
        read(scok_,buffer(buff_),boost::bind(&talk_to_svr::read_complete,this,_1,_2););
        process_msg();
    }
    void process_msg(){
        std::string msg(buff_,already_read_);
        if(msg.find("login")==0) on_login(msg);
        else if (msg.find("ping")==0) on_ping(msg);
        else if (msg.find("clients")==0) on_clients(msg);
        else std::cerr << "invalid msg" << msg << std::endl;
    }

    void on_login() { do_ask_clients();}
    void on_ping(const std::string & msg){
        std::istringstream in(msg);
        std::string answer;
        in >> answer << answer;
        if(answer == "client_list_changed")
            do_ask_clients();
    }

    void on_clients(const std::string &msg){
        std::string clients=msg.substr(8);
        std::cout << username_ << ", new clients list:" << clients;
    }

    void do_ask_clients(){
        write("ask_clients\n");
        read_answer();
    }

    void write(const std::string& msg) { sock_.write_some(buffer(msg));}
    size_t read_complete(const boost::system::error_code& error,size_t bytes){
    }
};
