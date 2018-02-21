/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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

#ifndef DROPFROM_H
#define DROPFROM_H

#include "ns3/queue.h"

namespace ns3 {

/**
 * \ingroup queue
 *
 * \brief A FIFO packet queue that drops packets from a position in queue
 */
template <typename Item>
class DropFromQueue : public Queue<Item>
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  /**
   * \brief DropFromQueue Constructor
   *
   * Creates a dropfrom queue with a maximum size of 100 packets by default
   */
  DropFromQueue ();
  virtual ~DropFromQueue ();
  virtual bool Enqueue (Ptr<Item> item);
  virtual Ptr<Item> Dequeue (void);
  virtual Ptr<Item> Remove (void);
  virtual Ptr<const Item> Peek (void) const;
  virtual Ptr<const Item> PeekAt (uint32_t) const;
  virtual bool EnqueueAt (uint32_t,Ptr<Item> item);

private:
  using Queue<Item>::Head;
  using Queue<Item>::Tail;
  using Queue<Item>::DoEnqueue;
  using Queue<Item>::DoDequeue;
  using Queue<Item>::DoRemove;
  using Queue<Item>::DoPeek;
};


/**
 * Implementation of the templates declared above.
 */

template <typename Item>
TypeId
DropFromQueue<Item>::GetTypeId (void)
{
  static TypeId tid = TypeId (("ns3::DropFromQueue<" + GetTypeParamName<DropFromQueue<Item> > () + ">").c_str ())
    .SetParent<Queue<Item> > ()
    .SetGroupName ("Network")
    .template AddConstructor<DropFromQueue<Item> > ()
  ;
  return tid;
}

template <typename Item>
DropFromQueue<Item>::DropFromQueue ()
  : Queue<Item> ()
{

}

template <typename Item>
DropFromQueue<Item>::~DropFromQueue ()
{

}

template <typename Item>
bool
DropFromQueue<Item>::Enqueue (Ptr<Item> item)
{
  return DoEnqueue (Tail (), item);
}

template <typename Item>
Ptr<Item>
DropFromQueue<Item>::Dequeue (void)
{

  Ptr<Item> item = DoDequeue (Head ());

  return item;
}

template <typename Item>
Ptr<Item>
DropFromQueue<Item>::Remove (void)
{

  Ptr<Item> item = DoRemove (Head ());

  return item;
}

template <typename Item>
Ptr<const Item>
DropFromQueue<Item>::Peek (void) const
{
  return DoPeek (Head ());
}

template <typename Item>
Ptr<const Item>
DropFromQueue<Item>::PeekAt (uint32_t pos) const
{
  auto ptr = Head ();

  for (uint32_t i = 0; i < pos; i++)
    {
      ptr++;
    }

  Ptr<const Item> item = DoPeek (ptr);

  return item;
}

template <typename Item>
bool
DropFromQueue<Item>::EnqueueAt (uint32_t pos, Ptr<Item> item)
{

  auto ptr = Head ();

  for (uint32_t i = 0; i < pos; i++)
    {
      ptr++;
    }

  return DoEnqueue (ptr, item);

}

} // namespace ns3

#endif /* DROPFROM_H */
