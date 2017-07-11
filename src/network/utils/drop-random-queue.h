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

#ifndef DROPRANDOM_H
#define DROPRANDOM_H

#include "ns3/queue.h"

namespace ns3 {

/**
 * \ingroup queue
 *
 * \brief A FIFO packet queue that drops tail-end packets on overflow
 */
template <typename Item>
class DropRandomQueue : public Queue<Item>
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  /**
   * \brief DropRandomQueue Constructor
   *
   * Creates a droprandom queue with a maximum size of 100 packets by default
   */
  DropRandomQueue ();

  virtual ~DropRandomQueue ();

  virtual bool Enqueue (Ptr<Item> item);
  virtual Ptr<Item> Dequeue (void);
  virtual Ptr<Item> Remove (void);
  virtual Ptr<const Item> Peek (void) const;
  Ptr<Item> RemoveRandom (uint32_t);
  bool EnqueueRandom (uint32_t,Ptr<Item> item);

private:
  using Queue<Item>::Head;
  using Queue<Item>::Tail;
  using Queue<Item>::DoEnqueue;
  using Queue<Item>::DoDequeue;
  using Queue<Item>::DoRemove;
  using Queue<Item>::DoPeek;

  NS_LOG_TEMPLATE_DECLARE;     //!< redefinition of the log component
};


/**
 * Implementation of the templates declared above.
 */

template <typename Item>
TypeId
DropRandomQueue<Item>::GetTypeId (void)
{
  static TypeId tid = TypeId (("ns3::DropRandomQueue<" + GetTypeParamName<DropRandomQueue<Item> > () + ">").c_str ())
    .SetParent<Queue<Item> > ()
    .SetGroupName ("Network")
    .template AddConstructor<DropRandomQueue<Item> > ()
  ;
  return tid;
}

template <typename Item>
DropRandomQueue<Item>::DropRandomQueue () :
  Queue<Item> (),
  NS_LOG_TEMPLATE_DEFINE ("DropRandomQueue")
{
  NS_LOG_FUNCTION (this);
}

template <typename Item>
DropRandomQueue<Item>::~DropRandomQueue ()
{
  NS_LOG_FUNCTION (this);
}

template <typename Item>
bool
DropRandomQueue<Item>::Enqueue (Ptr<Item> item)
{
  NS_LOG_FUNCTION (this << item);

  return DoEnqueue (Tail (), item);
}

template <typename Item>
Ptr<Item>
DropRandomQueue<Item>::Dequeue (void)
{
  NS_LOG_FUNCTION (this);

  Ptr<Item> item = DoDequeue (Head ());

  NS_LOG_LOGIC ("Popped " << item);

  return item;
}


template <typename Item>
Ptr<Item>
DropRandomQueue<Item>::Remove (void)
{
  NS_LOG_FUNCTION (this);

  Ptr<Item> item = DoRemove (Head ());

  NS_LOG_LOGIC ("Removed " << item);

  return item;
}

template <typename Item>
Ptr<const Item>
DropRandomQueue<Item>::Peek (void) const
{
  NS_LOG_FUNCTION (this);

  return DoPeek (Head ());
}

template <typename Item>
bool
DropRandomQueue<Item>::EnqueueRandom (uint32_t pos,Ptr<Item> item)
{

auto ptr = Head ();
  for (uint32_t i = 1; i < pos; i++)
    {
      ptr++;
    }
  return DoEnqueue (ptr, item);

}

template <typename Item>
Ptr<Item>
DropRandomQueue<Item>::RemoveRandom (uint32_t pos)
{
  auto ptr = Head ();
  for (uint32_t i = 0; i < pos; i++)
    {
      ptr++;
    }
  Ptr<Item> item = DoRemove (ptr);
  return item;
}

} // namespace ns3

#endif /* DROPRANDOM_H */
