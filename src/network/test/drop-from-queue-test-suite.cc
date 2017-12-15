/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 University of Washington
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
 */

#include "ns3/test.h"
#include "ns3/drop-from-queue.h"
#include "ns3/uinteger.h"
#include "ns3/queue-disc.h"

using namespace ns3;

/**
 * \ingroup network-test
 * \ingroup tests
 *
 * DropfromQueue unit tests.
 */
class DropFromQueueTestCase : public TestCase
{
public:
  DropFromQueueTestCase ();
  virtual void DoRun (void);
};

DropFromQueueTestCase::DropFromQueueTestCase ()
  : TestCase ("Sanity check on the drop from queue implementation")
{
}
void
DropFromQueueTestCase::DoRun (void)
{
  Ptr<DropFromQueue<Packet> > queue = CreateObject<DropFromQueue<Packet> > ();
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MaxPackets", UintegerValue (3)), true,
                         "Verify that we can actually set the attribute");

  Ptr<Packet> p1, p2, p3, p4;
  p1 = Create<Packet> ();
  p2 = Create<Packet> ();
  p3 = Create<Packet> ();
  p4 = Create<Packet> ();

  NS_TEST_EXPECT_MSG_EQ (queue->GetNPackets (), 0, "There should be no packets in there");
  queue->Enqueue (p1);
  NS_TEST_EXPECT_MSG_EQ (queue->GetNPackets (), 1, "There should be one packet in there");
  queue->Enqueue (p2);
  NS_TEST_EXPECT_MSG_EQ (queue->GetNPackets (), 2, "There should be two packets in there");
  queue->Enqueue (p3);
  NS_TEST_EXPECT_MSG_EQ (queue->GetNPackets (), 3, "There should be three packets in there");
  queue->Enqueue (p4); // will be dropped
  NS_TEST_EXPECT_MSG_EQ (queue->GetNPackets (), 3, "There should be still three packets in there");

  Ptr<Packet> packet;

  packet = queue->Dequeue ();
  NS_TEST_EXPECT_MSG_EQ ((packet != 0), true, "I want to remove the first packet");
  NS_TEST_EXPECT_MSG_EQ (queue->GetNPackets (), 2, "There should be two packets in there");
  NS_TEST_EXPECT_MSG_EQ (packet->GetUid (), p1->GetUid (), "was this the first packet ?");

  packet = queue->Dequeue ();
  NS_TEST_EXPECT_MSG_EQ ((packet != 0), true, "I want to remove the second packet");
  NS_TEST_EXPECT_MSG_EQ (queue->GetNPackets (), 1, "There should be one packet in there");
  NS_TEST_EXPECT_MSG_EQ (packet->GetUid (), p2->GetUid (), "Was this the second packet ?");

  packet = queue->Dequeue ();
  NS_TEST_EXPECT_MSG_EQ ((packet != 0), true, "I want to remove the third packet");
  NS_TEST_EXPECT_MSG_EQ (queue->GetNPackets (), 0, "There should be no packets in there");
  NS_TEST_EXPECT_MSG_EQ (packet->GetUid (), p3->GetUid (), "Was this the third packet ?");

  packet = queue->Dequeue ();
  NS_TEST_EXPECT_MSG_EQ ((packet == 0), true, "There are really no packets in there");
}

/**
 * \ingroup network-test
 * \ingroup tests
 *
 * \brief DropFrom Queue TestSuite
 */
class DropFromQueueTestSuite : public TestSuite
{
public:
  DropFromQueueTestSuite ()
    : TestSuite ("drop-from-queue", UNIT)
  {
    AddTestCase (new DropFromQueueTestCase (), TestCase::QUICK);
  }
};

static DropFromQueueTestSuite g_dropFromQueueTestSuite; //!< Static variable for test initialization
