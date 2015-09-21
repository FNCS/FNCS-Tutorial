/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/nix-vector-routing-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/point-to-point-dumbbell.h"
#include "ns3/bridge-helper.h"
#include "ns3/wifi-module.h"
#include "ns3/netanim-module.h"
#include "ns3/wimax-helper.h"
#include "ns3/lte-helper.h"
#include "ns3/ipv4.h"
#include "ns3/fncs-application.h"
#include "ns3/fncs-application-helper.h"
#include "ns3/fncs-simulator-impl.h"

#include <cstdlib>
#include <vector>
#include <map>
#include <iostream>
#include <stdexcept>

using namespace ns3;
using namespace std;

struct MarketModel{
    string nodeNamePrefixes;
    string connectedBus;
    int numberofnodes;
    string marketname;       
};

vector<MarketModel*> parseLinkModel(string name, int &totalNumberOfNodes){

    vector<MarketModel*> toReturn;
    totalNumberOfNodes=0;
    ifstream inputFile(name.c_str());
    if(!inputFile.good())
        throw std::invalid_argument("Cannot open file!");

    MarketModel *toAdd;
    while(!inputFile.eof()){
        toAdd=new MarketModel();
        inputFile >> toAdd->numberofnodes >> toAdd->marketname >> toAdd->nodeNamePrefixes >> toAdd->connectedBus;
        if(toAdd->numberofnodes==0) //ifstream zicti
            continue;
        totalNumberOfNodes+=toAdd->numberofnodes;
        toReturn.push_back(toAdd);
    }
    return toReturn; 

}

map<string, string> getMarketControllerMap(vector<MarketModel *> networks){
    map<string, string> MarketToControllerMap;

    MarketModel *market;
    string mName, nodePrefix;
    // Assuming market names are going to be unique
    for( int i =0; i < networks.size(); i++) {
        market = networks[i];
        MarketToControllerMap[networks[i]->marketname] = networks[i]->nodeNamePrefixes;
    }
    return MarketToControllerMap;
}
    int
main (int argc, char *argv[])
{
    if(argc < 2){
        cout << "Usage ./first <power grid link model file name>" << endl;
        return 0;
    }
    int totalNumberOfNodes=0;

    /* reads the list of market models from the linkModel file*/

    /* Each market model is defined by its 
     * size (Number of HVAC/controllers)
     * Name of Market (MarkNIF)
     * Prefix of Node Names ( Prefix for controller name)
     * the busid of the bus, market is connected to
     */
    /* totalNumberOfNodes is the sum of size of all market model */
    vector<MarketModel *> networks=parseLinkModel(string(argv[1]),totalNumberOfNodes);

    cout << "Creating " << networks.size() << " networks with " << totalNumberOfNodes << " nodes in total" << endl;

    map<string, string> marketToControllerMap = getMarketControllerMap(networks);
#if 0
    for(map<string, string>::iterator it=marketToControllerMap.begin(); it!=marketToControllerMap.end(); it++){
        cout<<" For market " << it->first << " , nodePrefix = " << it->second << "\n";
    }
#endif
    int createNodes=(totalNumberOfNodes/20)+1; //we create groups of 20 nodes;

    FncsSimulatorImpl *hb=new FncsSimulatorImpl();
    Ptr<FncsSimulatorImpl> hb2(hb);
    hb->Unref();

    Simulator::SetImplementation(hb2);

    Ipv4NixVectorHelper nixRouting;
    Ipv4StaticRoutingHelper staticRouting;

    LogComponentEnable ("FncsApplication", LOG_LEVEL_INFO);


    Ipv4AddressHelper addresses;

    InternetStackHelper ihelper;

    Ipv4ListRoutingHelper list; 
    list.Add (staticRouting, 0); 
    list.Add (nixRouting, 10);

    ihelper.SetRoutingHelper (list);

    /*Create CMSA nodes*/
    NodeContainer csma[createNodes],markCon;

    for(int i=0;i<createNodes;i++){
        csma[i].Create(21);
    }
    //create market
    markCon.Create(networks.size());

    ihelper.Install(markCon);
    CsmaHelper chelper;

    chelper.SetChannelAttribute ("DataRate", DataRateValue (10000000));
    chelper.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (3)));


    NetDeviceContainer csmaDevices[createNodes];

    for(int i=0;i<createNodes;i++){
        stringstream ip;
        ip << "10.1." << i << ".0";

        csmaDevices[i]=chelper.Install(csma[i]);

        ihelper.Install(csma[i]);
        addresses.SetBase(ip.str().c_str(),"255.255.255.0");
        addresses.Assign(csmaDevices[i]);
        ip.str(string());
    }

    PointToPointHelper phelper3;
    phelper3.SetDeviceAttribute ("DataRate", StringValue ("4Mbps"));
    phelper3.SetChannelAttribute ("Delay", StringValue ("2ms"));

    //connect market nodes to csma nodes.
    for(int j=0;j<networks.size();j++){
        for(int i=0;i<createNodes;i++){
            stringstream ip;
            ip << "11." << j+1 << "." << i+1 << ".0";

            NetDeviceContainer csma1dbell1=phelper3.Install(markCon.Get(j),csma[i].Get(0));

            addresses.SetBase(ip.str().c_str(),"255.255.255.0");
            addresses.Assign(csma1dbell1);

            ip.str(string());

        }
    }


    //Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    NodeContainer gldnodes;

    //mark market nodes for fncs app installation.
    //gldnodes.Add(markCon);
    //gldnodes.Add(csma[0].Get(21));

    {
        //add the CMSA nodes
        int installNodeCount=totalNumberOfNodes/20;
        int i=0;
        for(;i<installNodeCount;i++)
            for(int j=1;j<21;j++){
                gldnodes.Add(csma[i].Get(j));
            }
        installNodeCount=totalNumberOfNodes%20;
        for(int l=0;l<installNodeCount;l++){
            gldnodes.Add(csma[i].Get(l));
        }//i goes out of scope so I can reuse it later.
    }

    /* Set up a vector containing names of all objects on the network */
    /* The lits includes market names and all controller names */
    vector<string> names;

    stringstream ss;

    //add market names
    for(int i=0;i<networks.size();i++){
        names.push_back(networks[i]->marketname);
    }

    //add houses
    for(int i=0;i<networks.size();i++){
        for(int k=1;k<=networks[i]->numberofnodes;k++){
            ss << networks[i]->nodeNamePrefixes << k;
            names.push_back(ss.str());
            //cout << ss.str() << endl;
            ss.str(std::string());
        }
    }

    /* the '1' below is the offset where number of nodes begins */
    FncsApplicationHelper help(networks[0]->nodeNamePrefixes, 1);
    /* Passes the names of all nodes/objects on the network, and list of all nodes*/ 
    /* Calls fncs application helper object ( defined in ns3home/src/applications/helper/fncsApplicationHelper.
     * This creates a map of (name,ipaddreses) and links it to the list <FNCS_applications> contained in teh ApplicationContainer fncsaps */
    ApplicationContainer fncsaps= help.Install(gldnodes);
    {
        size_t index = 0;
        for (NodeContainer::Iterator i = markCon.Begin();
                i != markCon.End();
                ++i)
        {
            fncsaps.Add(help.Install(*i, networks[index++]->marketname));
        }
    }
    fncsaps.Start (Seconds (0.0));
    fncsaps.Stop (Seconds (259200.0));

    Simulator::Run ();
    Simulator::Destroy ();
}
