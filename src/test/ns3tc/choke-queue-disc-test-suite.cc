//* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2017 NITK Surathkal
 *
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
 *
 * Authors: Nandita G <gm.nandita@gmail.com>
 *          Mohit P. Tahiliani <tahiliani@nitk.edu.in>
 *
 */

#include "ns3/test.h"
#include "ns3/simulator.h"
#include "ns3/choke-queue-disc.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv4-packet-filter.h"
#include "ns3/ipv4-queue-disc-item.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv6-header.h"
#include "ns3/ipv6-packet-filter.h"
#include "ns3/ipv6-queue-disc-item.h"
#include "ns3/tcp-header.h"
#include "ns3/udp-header.h"
#include "ns3/uinteger.h"
#include "ns3/pointer.h"
#include "ns3/fq-codel-queue-disc.h"

using namespace ns3;

/**
 * This class tests packets for which there is no suitable filter
 */

/**
 * This class tests the IP flows separation and the packet limit
 *
*/

class ChokeQueueDiscNoSuitableFilter : public TestCase
{
public:
  ChokeQueueDiscNoSuitableFilter ();
  virtual ~ChokeQueueDiscNoSuitableFilter ();

private:
  virtual void DoRun (void);
};

ChokeQueueDiscNoSuitableFilter::ChokeQueueDiscNoSuitableFilter ()
  : TestCase ("Test packets that are not classified by any filter")
{
}

ChokeQueueDiscNoSuitableFilter::~ChokeQueueDiscNoSuitableFilter ()
{
}

void
ChokeQueueDiscNoSuitableFilter::DoRun (void)
{
  // Packets that cannot be classified by the available filters should be dropped
  Ptr<ChokeQueueDisc> queueDisc = CreateObjectWithAttributes<ChokeQueueDisc> ();
  Ptr<FqCoDelIpv4PacketFilter> filter = CreateObject<FqCoDelIpv4PacketFilter> ();
  queueDisc->AddPacketFilter (filter);
  queueDisc->Initialize ();

  Ptr<Packet> p;
  p = Create<Packet> ();
  Ptr<Ipv6QueueDiscItem> item;
  Ipv6Header ipv6Header;
  Address dest;
  item = Create<Ipv6QueueDiscItem> (p, dest, 0, ipv6Header);
  queueDisc->Enqueue (item);
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetNQueueDiscClasses (), 0, "no flow queue should have been created");

  p = Create<Packet> (reinterpret_cast<const uint8_t*> ("hello, world"), 12);
  item = Create<Ipv6QueueDiscItem> (p, dest, 0, ipv6Header);
  queueDisc->Enqueue (item);
  NS_TEST_ASSERT_MSG_EQ (queueDisc->GetNQueueDiscClasses (), 0, "no flow queue should have been created");

  Simulator::Destroy ();
}

class ChokeQueueDiscResponseToPassiveAndAggressiveFlows : public TestCase
{
public:
  ChokeQueueDiscResponseToPassiveAndAggressiveFlows ();
  virtual ~ChokeQueueDiscResponseToPassiveAndAggressiveFlows ();

private:
  virtual void DoRun (void);
  void AddPacket (Ptr<ChokeQueueDisc> queue, Ipv4Header hdr);
};

ChokeQueueDiscResponseToPassiveAndAggressiveFlows::ChokeQueueDiscResponseToPassiveAndAggressiveFlows ()
  : TestCase ("Compare response to passive flows and aggressive flows")
{
}

ChokeQueueDiscResponseToPassiveAndAggressiveFlows::~ChokeQueueDiscResponseToPassiveAndAggressiveFlows ()
{
}

void
ChokeQueueDiscResponseToPassiveAndAggressiveFlows::AddPacket (Ptr<ChokeQueueDisc> queue, Ipv4Header hdr)
{
  Ptr<Packet> p = Create<Packet> (100);
  Address dest;
  Ptr<Ipv4QueueDiscItem> item = Create<Ipv4QueueDiscItem> (p, dest, 0, hdr);
  queue->Enqueue (item);
}

void
ChokeQueueDiscResponseToPassiveAndAggressiveFlows::DoRun (void)
{
  Ptr<ChokeQueueDisc> queueDisc = CreateObject<ChokeQueueDisc> ();  
  Ptr<FqCoDelIpv4PacketFilter> Ipv4PacketFilter = CreateObject<FqCoDelIpv4PacketFilter> ();
  Ptr<FqCoDelIpv6PacketFilter> Ipv6PacketFilter = CreateObject<FqCoDelIpv6PacketFilter> ();
  queueDisc->AddPacketFilter (Ipv4PacketFilter);
  queueDisc->AddPacketFilter (Ipv6PacketFilter);
  NS_TEST_EXPECT_MSG_EQ (queueDisc->SetAttributeFailSafe ("MinTh", DoubleValue (70)), true,
                         "Verify that we can actually set the attribute MinTh");
  NS_TEST_EXPECT_MSG_EQ (queueDisc->SetAttributeFailSafe ("MaxTh", DoubleValue (150)), true,
                         "Verify that we can actually set the attribute MaxTh");
  NS_TEST_EXPECT_MSG_EQ (queueDisc->SetAttributeFailSafe ("QueueLimit", UintegerValue (300)), true,
                         "Verify that we can actually set the attribute QueueLimit");
  queueDisc->Initialize ();

  Ipv4Header hdr;
  hdr.SetPayloadSize (100);
  hdr.SetSource (Ipv4Address ("10.10.1.1"));
  hdr.SetDestination (Ipv4Address ("10.10.1.2"));
  hdr.SetProtocol (7);

  for(uint32_t i=0;i<300;i++)
  {
  AddPacket (queueDisc, hdr);
  }
  ChokeQueueDisc::Stats st = StaticCast<ChokeQueueDisc> (queueDisc)->GetStats ();
  int32_t test1 = st.unforcedDrop + st.forcedDrop + st.qLimDrop + st.randomDrop; 

  queueDisc = CreateObject<ChokeQueueDisc> (); 
  queueDisc->AddPacketFilter (Ipv4PacketFilter);
  queueDisc->AddPacketFilter (Ipv6PacketFilter);
  NS_TEST_EXPECT_MSG_EQ (queueDisc->SetAttributeFailSafe ("MinTh", DoubleValue (70)), true,
                         "Verify that we can actually set the attribute MinTh");
  NS_TEST_EXPECT_MSG_EQ (queueDisc->SetAttributeFailSafe ("MaxTh", DoubleValue (150)), true,
                         "Verify that we can actually set the attribute MaxTh");
  NS_TEST_EXPECT_MSG_EQ (queueDisc->SetAttributeFailSafe ("QueueLimit", UintegerValue (300)), true,
                         "Verify that we can actually set the attribute QueueLimit");
  queueDisc->Initialize ();

  Ipv4Header hdr1;
  hdr1.SetPayloadSize (100);
  hdr1.SetSource (Ipv4Address ("10.10.1.1"));
  hdr1.SetDestination (Ipv4Address ("10.10.1.7"));
  hdr1.SetProtocol (7);

  Ipv4Header hdr2;
  hdr2.SetPayloadSize (100);
  hdr2.SetSource (Ipv4Address ("10.10.1.1"));
  hdr2.SetDestination (Ipv4Address ("10.10.1.6"));
  hdr2.SetProtocol (7);

  Ipv4Header hdr3;
  hdr3.SetPayloadSize (100);
  hdr3.SetSource (Ipv4Address ("10.10.1.1"));
  hdr3.SetDestination (Ipv4Address ("10.10.1.5"));
  hdr3.SetProtocol (7);

  Ipv4Header hdr4;
  hdr4.SetPayloadSize (100);
  hdr4.SetSource (Ipv4Address ("10.10.1.1"));
  hdr4.SetDestination (Ipv4Address ("10.10.1.4"));
  hdr4.SetProtocol (7);

  for (uint32_t j=0;j<75;j++)
    {
      AddPacket (queueDisc, hdr1);
      AddPacket (queueDisc, hdr2);     
      AddPacket (queueDisc, hdr3);
      AddPacket (queueDisc, hdr4);      
    }
  ChokeQueueDisc::Stats st1 = StaticCast<ChokeQueueDisc> (queueDisc)->GetStats ();
  int32_t test2 = st1.unforcedDrop + st1.forcedDrop + st1.qLimDrop + st1.randomDrop;
  NS_TEST_EXPECT_MSG_GT_OR_EQ (test1, test2, "Test1 should have more or equal number of drops compared t test2");
  Simulator::Destroy ();
}


class ChokeQueueDiscTestSuite : public TestSuite
{
public:
  ChokeQueueDiscTestSuite ();
};

ChokeQueueDiscTestSuite::ChokeQueueDiscTestSuite ()
  : TestSuite ("choke-test", UNIT)
{
  AddTestCase (new ChokeQueueDiscNoSuitableFilter, TestCase::QUICK);
  AddTestCase (new ChokeQueueDiscResponseToPassiveAndAggressiveFlows, TestCase::QUICK);
}

static ChokeQueueDiscTestSuite ChokeQueueDiscTestSuite;
