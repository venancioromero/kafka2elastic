#include <iostream>
#include <librdkafka/rdkafkacpp.h>
#include <libconfig.h++>
#include <string>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

using namespace std;
using namespace libconfig;

int main() {

    // read properties
    Config cfg;
    cfg.readFile("/home/venoty/CLionProjects/kafka2elastic/kafka2elastic.properties");
    const Setting& props = cfg.getRoot();

    // Kafka Variables
    const char *brokers  = props["brokers"];
    string topics        = props["topics"];
    const char *group_id = props["group_id"];

    vector<std::string> topics_vector;

    boost::split(topics_vector, topics, boost::is_any_of(","), boost::token_compress_on);

    // ES Variables
    const char *es_host  = props["es_host"];

    string errstr;

    RdKafka::Conf *conf  = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    //RdKafka::Conf *tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);

    if (conf->set("metadata.broker.list", brokers, errstr) != RdKafka::Conf::CONF_OK){
        cerr << errstr << endl;
        exit(1);
    }

    if (conf->set("group.id",  group_id, errstr) != RdKafka::Conf::CONF_OK) {
        cerr << errstr << endl;
        exit(1);
    }

    RdKafka::KafkaConsumer *consumer = RdKafka::KafkaConsumer::create(conf, errstr);

    if (!consumer) {
        std::cerr << "Failed to create consumer: " << errstr << std::endl;
        exit(1);
    }

    delete conf;

    std::cout << "% Created consumer " << consumer->name() << std::endl;

    RdKafka::ErrorCode err = consumer->subscribe(topics_vector);

    if (err) {
        cerr << "Failed to subscribe to " << topics_vector.size() << " topics: " << RdKafka::err2str(err) << std::endl;
        exit(1);
    }
    while (true) {
        RdKafka::Message *msg = consumer->consume(1000);

        if (msg->err() == RdKafka::ERR_NO_ERROR)
            printf("%.*s\n",static_cast<int>(msg->len()),static_cast<const char *>(msg->payload()));
        delete msg;
    }
    return 0;
}