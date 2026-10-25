#include <iostream>
using namespace std;


// Adaptee (Third Party Provider)
class XmlDataProvider {
public:
    string getXMLData(string& data){
        int sep = data.find(':');
        string id = data.substr(0, sep);
        string name = data.substr(sep+1);
        string res = "<person><id>" + id + "</id><name>" + name + "</name></person>";
        return res;
    }
};

class IReports {
public:
    virtual string getJsonData(string& data) = 0;
    virtual ~IReports() = default;
};

class XmlDataProviderAdapter: public IReports {
    XmlDataProvider* provider;
public:
    XmlDataProviderAdapter(XmlDataProvider* provider): provider(provider){}

    string getJsonData(string& data) override {
        string xml = provider->getXMLData(data);

        int idstart = xml.find("<id>") + 4;
        int idend = xml.find("</id>");
        string id = xml.substr(idstart, idend - idstart);

        int namestart = xml.find("<name>") + 6;
        int nameend = xml.find("</name>");
        string name = xml.substr(namestart, nameend - namestart);

        string json = "{\"id\": \"" + id + "\", \"name\": \"" + name + "\"}";
        return json;
    }
};

class Client {
public:
    void printReport(IReports* reportService, string data){
        string res = reportService->getJsonData(data);
        cout<<res<<endl;
    }
};

int main(){
    XmlDataProvider* xmlService = new XmlDataProvider();
    IReports* adapter = new XmlDataProviderAdapter(xmlService);

    Client client;
    string input = "101:Alice";
    client.printReport(adapter, input);

    delete adapter;
    delete xmlService;

    return 0;
}

