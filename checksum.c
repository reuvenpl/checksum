/* Change the routing */
ip_header->daddr = <my_ip>; //change to yours IP
tcp_header->dest = <my_port>; //change to yours listening port

/* Fix IP header checksum */
ip_header->check = 0;
ip_header->check = ip_fast_csum((u8 *)ip_header, ip_header->ihl);

/*
 * From Linux doc here: https://elixir.bootlin.com/linux/v4.15/source/include/linux/skbuff.h#L90
 * CHECKSUM_NONE:
 *
 *   Device did not checksum this packet e.g. due to lack of capabilities.
 *   The packet contains full (though not verified) checksum in packet but
 *   not in skb->csum. Thus, skb->csum is undefined in this case.
 */
skb->ip_summed = CHECKSUM_NONE;
skb->csum_valid = 0;

/* Linearize the skb */
if (skb_linearize(skb) < 0) {
	/* Handle error
}

/* Re-take headers. The linearize may change skb's pointers */
ip_header = ip_hdr(skb);
tcp_header = tcp_hdr(skb);

/* Fix TCP header checksum */
tcplen = (ntohs(ip_header->tot_len) - ((ip_header->ihl) << 2));
tcp_header->check = 0;
tcp_header->check = tcp_v4_check(tcplen, ip_header->saddr, ip_header->daddr, csum_partial((char *)tcp_header, tcplen, 0));