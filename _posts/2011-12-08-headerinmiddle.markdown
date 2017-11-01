---
layout: post
title: "ns3: Touching a header in the middle of a packet"
date: 2011-12-08 00:03:14 -0500
tags: code
---
A trick from <http://matinrad.com/index.php/2010/08/how-to-find-a-packet-header-in-ns-3/>

The following example was my first try to toggle the ECN bit in RED queue. RED queue does not know about the packet's structure. So we search for the first IP header, then toggle the ECN bit:

```c++
// assume the packet is Ptr<Packet> p
PacketMetadata::ItemIterator metadataIterator = p->BeginItem();
PacketMetadata::Item item;
while (metadataIterator.HasNext())
  {
    item = metadataIterator.Next();
    NS_LOG_FUNCTION("item name: " << item.tid.GetName());
    if(item.tid.GetName() == "ns3::Ipv4Header")
      {
        NS_LOG_DEBUG("IPv4 Header Found");
        Callback<ObjectBase*> constructor = item.tid.GetConstructor();
        NS_ASSERT(!constructor.IsNull());
        // Ptr<> and DynamicCast<> won't work here as all headers are from ObjectBase, not Object
        ObjectBase *instance = constructor();
        NS_ASSERT(instance != 0);
        Ipv4Header *ipv4Header = dynamic_cast<Ipv4Header*> (instance);
        NS_ASSERT(ipv4Header != 0);
        ipv4Header->Deserialize(item.current);
        // If ECN-capable and not marked, mark it and write to the buffer
        if (ipv4Header->GetEcn () == Ipv4Header::ECT0 || ipv4Header->GetEcn () == Ipv4Header::ECT1)
          {
            ipv4Header->SetEcn (Ipv4Header::CE);
            ipv4Header->Serialize (item.current);
          }
        marked = (ipv4Header->GetEcn () == Ipv4Header::CE);
        delete ipv4Header;
        break;
      }
  }
```

This fails at link time because now NS3 is modularised. We can't use anything from internet module at network module or otherwise a dependency loop is created.
