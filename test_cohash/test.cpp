#include <iostream>
#include <string>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <map>
#include <cmath>
#include <set>
#include <initializer_list>
#include <vector>
#include <algorithm>

template<typename K, typename V> class Hash;
template<typename N> class VNode;

template<>
class Hash<std::string, uint32_t> {
public:
	uint32_t operator()(const std::string & key) const {
		int p = 16777619;
		uint32_t hash = 2166136261L;
		for (int i = 0, num = key.size(); i < num; ++i) {
			hash = (hash ^ key[i]) * p;
		}
		hash += hash << 13;
		hash ^= hash >> 7;
		hash += hash << 3;
		hash ^= hash >> 17;
		hash += hash << 5;

		if (hash < 0) {
			hash = abs((long)hash);
		}
		return hash;
	}
};

//class Node {
//public:
//	Node() {};
//	virtual ~Node() {};
//	virtual operator std::string() const = 0;
//	virtual bool operator==(const Node & n) = 0;
//};

template<typename V>
class HashRingAlgorithm {
public:
	HashRingAlgorithm() {}
	virtual ~HashRingAlgorithm() {}
public:
	uint32_t hash(const V & v) const {
		Hash<V, uint32_t> h;
		return h(v);
	}
};

/*
 * 虚拟节点
*/
template<typename N>
class VNode {
public:
	VNode() {};//just make compiler happy
	VNode(const N & n, uint32_t idx) {
		key_ = std::string(n) + "#" + std::to_string(idx);
		rnode_ = n;
	};

	virtual ~VNode() {

	};

	operator std::string() const {
		return key_;
	}

	N getRNode() const {
		return rnode_;
	}
private:
	std::string key_;
	N rnode_;//真实节点
};


/*
 * 哈希环
*/
template<typename N, typename K>
class ConsitentHashRing {
public:
	ConsitentHashRing(uint32_t vn_count = 1000) {
		vn_count_ = vn_count;
	}

	virtual ~ConsitentHashRing() {
		vnodes_map_.clear();
	}
public:
	void addNode(const N & n) {
		for (uint32_t i = 0; i < vn_count_; i++) {
			VNode<N> vnode(n, i);
			uint32_t hash = hash_algorithm_.hash(vnode);
			vnodes_map_[hash] = vnode;
			real_nodes_.push_back(n);
		}
	}

	N  getNode(const K & v) const {
		uint32_t hash = hash_algorithm_.hash(v);
		auto it = vnodes_map_.lower_bound(hash);
		if (it == vnodes_map_.end()) {
			return vnodes_map_.begin()->second.getRNode();
		}
		return it->second.getRNode();
	}

	int removeNode(const N & n) {
		for (auto it = vnodes_map_.begin(); it != vnodes_map_.end();) {
			if (it->second.getRNode() == n) {
				vnodes_map_.erase(it++);
			}
			else {
				it++;
			}
		}

		for (auto it = real_nodes_.begin(); it != real_nodes_.end(); it++) {
			if (*it == n) {
				real_nodes_.erase(it);
				break;
			}
		}
		return 0;
	}

private:
	uint32_t vn_count_;
	std::map<uint32_t, VNode<N>> vnodes_map_;
	std::vector<N> real_nodes_;
	HashRingAlgorithm<K> hash_algorithm_;
};

class ServerNode {
public:
	ServerNode() {};
	ServerNode(const std::string & host, uint16_t port) {
		host_ = host;
		port_ = port;
	}
	virtual ~ServerNode() {}
	bool operator==(const ServerNode & s) const {
		if (s.host_ == host_ && s.port_ == port_) {
			return true;
		}
		return false;
	}
	operator std::string() const {
		return host_ + ":" + std::to_string(port_);
	}
public:
	std::string host_;
	uint16_t port_;
};

int main(char argc, char *argv[]) {
	ConsitentHashRing<ServerNode, std::string> hash_ring;
	hash_ring.addNode(ServerNode("192.168.128.1", 1935));
	hash_ring.addNode(ServerNode("192.168.128.2", 1935));
	hash_ring.addNode(ServerNode("192.168.128.3", 1935));
	hash_ring.addNode(ServerNode("192.168.128.4", 1935));
	hash_ring.addNode(ServerNode("192.168.128.5", 1935));
	hash_ring.addNode(ServerNode("192.168.128.6", 1935));
	hash_ring.addNode(ServerNode("192.168.128.7", 1935));
	hash_ring.addNode(ServerNode("192.168.128.8", 1935));
	hash_ring.addNode(ServerNode("192.168.128.9", 1935));

	int count[10] = { 0 };
	for (int i = 0; i < 10000; i++) {
		int m = rand() % 1000000;
		auto s = hash_ring.getNode(std::to_string(m));
		std::cout << std::string(s) << std::endl;
		if (s.host_ == "192.168.128.1") {
			count[0]++;
		}
		else if (s.host_ == "192.168.128.2") {
			count[1]++;
		}
		else if (s.host_ == "192.168.128.3") {
			count[2]++;
		}
		else if (s.host_ == "192.168.128.4") {
			count[3]++;
		}
		else if (s.host_ == "192.168.128.5") {
			count[4]++;
		}
		else if (s.host_ == "192.168.128.6") {
			count[5]++;
		}
		else if (s.host_ == "192.168.128.7") {
			count[6]++;
		}
		else if (s.host_ == "192.168.128.8") {
			count[7]++;
		}
		else if (s.host_ == "192.168.128.9") {
			count[8]++;
		}
		else if (s.host_ == "192.168.128.10") {
			count[9]++;
		}
	}
	for (int i = 0; i < 10; i++) {
		std::cout << "count[" << i << "] = " << count[i] << std::endl;
		count[i] = 0;
	}
	getchar();
	hash_ring.addNode(ServerNode("192.168.128.10", 1935));
	for (int i = 0; i < 10000; i++) {
		int m = rand() % 1000000;
		auto s = hash_ring.getNode(std::to_string(m));
		std::cout << std::string(s) << std::endl;
		if (s.host_ == "192.168.128.1") {
			count[0]++;
		}
		else if (s.host_ == "192.168.128.2") {
			count[1]++;
		}
		else if (s.host_ == "192.168.128.3") {
			count[2]++;
		}
		else if (s.host_ == "192.168.128.4") {
			count[3]++;
		}
		else if (s.host_ == "192.168.128.5") {
			count[4]++;
		}
		else if (s.host_ == "192.168.128.6") {
			count[5]++;
		}
		else if (s.host_ == "192.168.128.7") {
			count[6]++;
		}
		else if (s.host_ == "192.168.128.8") {
			count[7]++;
		}
		else if (s.host_ == "192.168.128.9") {
			count[8]++;
		}
		else if (s.host_ == "192.168.128.10") {
			count[9]++;
		}
	}
	for (int i = 0; i < 10; i++) {
		std::cout << "count[" << i << "] = " << count[i] << std::endl;
		count[i] = 0;
	}

	getchar();
	hash_ring.removeNode(ServerNode("192.168.128.10", 1935));
	for (int i = 0; i < 10000; i++) {
		int m = rand() % 1000000;
		auto s = hash_ring.getNode(std::to_string(m));
		std::cout << std::string(s) << std::endl;
		if (s.host_ == "192.168.128.1") {
			count[0]++;
		}
		else if (s.host_ == "192.168.128.2") {
			count[1]++;
		}
		else if (s.host_ == "192.168.128.3") {
			count[2]++;
		}
		else if (s.host_ == "192.168.128.4") {
			count[3]++;
		}
		else if (s.host_ == "192.168.128.5") {
			count[4]++;
		}
		else if (s.host_ == "192.168.128.6") {
			count[5]++;
		}
		else if (s.host_ == "192.168.128.7") {
			count[6]++;
		}
		else if (s.host_ == "192.168.128.8") {
			count[7]++;
		}
		else if (s.host_ == "192.168.128.9") {
			count[8]++;
		}
		else if (s.host_ == "192.168.128.10") {
			count[9]++;
		}
	}
	for (int i = 0; i < 10; i++) {
		std::cout << "count[" << i << "] = " << count[i] << std::endl;
	}
	return 0;
}