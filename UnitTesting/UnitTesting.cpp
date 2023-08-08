#pragma once
#include "CppUnitTest.h"
#include "mysql_connection.h"
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/connection.h>
#include "../Deamon/ThreadOvens.h"
#include "../Deamon/OvenClass.h"
#include "../Deamon/ThreadOvens.cpp"
#include "../Deamon/OvenClass.cpp"
#include "../Deamon/CheckStatus.h"
#include "../Deamon/CheckStatus.cpp"
#include "../Deamon/OrderClass.h"
#include "../Deamon/OrderClass.cpp"
#include "../Deamon/OrderItems.h"
#include "../Deamon/OrderItems.cpp"
#include "../Deamon/ThreadDelivery.h"
#include "../Deamon/ThreadDelivery.cpp"
#include "../Deamon/DeliveryClass.cpp"
#include "../Deamon/DeliveryClass.h"
#include "../Deamon/GraphClass.h"
#include "../Deamon/GraphClass.cpp"
#include <cassert>
#include "boost/graph/dijkstra_shortest_paths.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include "UnitTesting.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace testing;



namespace UnitTesting {

	//CheckStatus.h
	TEST_CLASS(statustoString) {
public:
	TEST_METHOD(StatusToStringTest_ReturnsCorrectStringForValidInput) {
		// Valid inputs
		CheckStatus::status_enum status;

		// PENDING
		status = CheckStatus::status_enum::PENDING;
		std::string result = CheckStatus::toString(status);
		Assert::AreEqual("PENDING", result.c_str());

		// PROCESSING
		status = CheckStatus::status_enum::PROCESSING;
		result = CheckStatus::toString(status);
		Assert::AreEqual("PROCESSING", result.c_str());

		// IN_PREPARATION
		status = CheckStatus::status_enum::IN_PREPARATION;
		result = CheckStatus::toString(status);
		Assert::AreEqual("IN_PREPARATION", result.c_str());

		// IN_DELIVERY
		status = CheckStatus::status_enum::IN_DELIVERY;
		result = CheckStatus::toString(status);
		Assert::AreEqual("IN_DELIVERY", result.c_str());

		// DELIVERED
		status = CheckStatus::status_enum::DELIVERED;
		result = CheckStatus::toString(status);
		Assert::AreEqual("DELIVERED", result.c_str());

		// CANCELED
		status = CheckStatus::status_enum::CANCELED;
		result = CheckStatus::toString(status);
		Assert::AreEqual("CANCELED", result.c_str());

		// DONE
		status = CheckStatus::status_enum::DONE;
		result = CheckStatus::toString(status);
		Assert::AreEqual("DONE", result.c_str());

		// QUANTITY_IN_PROGRESS
		status = CheckStatus::status_enum::QUANTITY_IN_PROGRESS;
		result = CheckStatus::toString(status);
		Assert::AreEqual("QUANTITY_IN_PROGRESS", result.c_str());
	}

	TEST_METHOD(StatusToStringTest_ReturnsEmptyStringForInvalidInput) {
		CheckStatus::status_enum status;

		status = static_cast<CheckStatus::status_enum>(99);
		std::string result = CheckStatus::toString(status);
		Assert::AreEqual("", result.c_str());
	}

	TEST_METHOD(StatusToStringTest_ReturnsEmptyStringForDefaultCase) {
		CheckStatus::status_enum status;
		status = static_cast<CheckStatus::status_enum>(-1);
		std::string result = CheckStatus::toString(status);
		Assert::AreEqual("", result.c_str());
	}
	};
	

	//GraphClass.cpp
	TEST_CLASS(GraphClassTests){
public:
	TEST_METHOD(GraphClassTest_TestGraphGenerationNumber) {
		GraphClass graphClass;

		Graph g = graphClass.generateGraph();

		assert(boost::num_vertices(g), 4);
		assert(boost::num_edges(g), 4);
	}

	TEST_METHOD(GraphClassTest_TestGraphVertexProperties) {
		GraphClass graphClass;

		Graph g = graphClass.generateGraph();

		Graph::vertex_iterator vertexIt, vertexEnd;
		boost::tie(vertexIt, vertexEnd) = boost::vertices(g);

		for (; vertexIt != vertexEnd; ++vertexIt) {
			Graph::vertex_descriptor vd = *vertexIt;
			std::string address = g[vd].address;
			std::string phone = g[vd].phone;

			switch (g[vd].id) {
			case 0:
				EXPECT_EQ(address, "Strada PizzaAlex");
				EXPECT_EQ(phone, "07553623892");
				break;
			case 1:
				EXPECT_EQ(address, "Codlea, Brasov, 9 mai bl 14");
				EXPECT_EQ(phone, "0755632019");
				break;
			case 2:
				EXPECT_EQ(address, "Brasov, strada Lunga nr 12");
				EXPECT_EQ(phone, "0762736273");
				break;
			case 5:
				EXPECT_EQ(address, "Brasov, Strada Mare nr 5");
				EXPECT_EQ(phone, "0723626327");
				break;
			}
		}
	}
	TEST_METHOD(GraphClassTest_TestGraphEdgeProperties) {
		GraphClass graphClass;

		Graph g = graphClass.generateGraph();

		Graph::edge_iterator edgeIt, edgeEnd;
		boost::tie(edgeIt, edgeEnd) = boost::edges(g);

		for (; edgeIt != edgeEnd; ++edgeIt) {
			Graph::edge_descriptor ed = *edgeIt;
			int distance = g[ed].distance;
			double timeToDeliver = g[ed].time;

			switch (g[source(ed, g)].id) {
			case 0:
				if (g[target(ed, g)].id == 1) {
					EXPECT_EQ(distance, 12);
					EXPECT_EQ(timeToDeliver, 2.5 * 12);
				}
				else if (g[target(ed, g)].id == 2) {
					EXPECT_EQ(distance, 20);
					EXPECT_EQ(timeToDeliver, 2.5 * 23);
				}
				else if (g[target(ed, g)].id == 5) {
					EXPECT_EQ(distance, 30);
					EXPECT_EQ(timeToDeliver, 2.5 * 30);
				}
				break;
			case 1:
				if (g[target(ed, g)].id == 2) {
					EXPECT_EQ(distance, 12);
					EXPECT_EQ(timeToDeliver, 2.5 * 12);
				}
				else if (g[target(ed, g)].id == 5) {
					EXPECT_EQ(distance, 20);
					EXPECT_EQ(timeToDeliver, 2.5 * 20);
				}
				break;
			case 2:
				if (g[target(ed, g)].id == 5) {
					EXPECT_EQ(distance, 10);
					EXPECT_EQ(timeToDeliver, 2.5 * 10);
				}
				break;
			default:
				break;
			}
		}
	}
    TEST_METHOD(GraphClassTest_TestGraphGenerationNumberAndCorrect)
	{
		GraphClass graphObj;
		Graph graph = graphObj.generateGraph();
		assert(boost::num_vertices(graph), 4);
		assert(boost::num_edges(graph), 5);

		EXPECT_EQ(graph[0].address, "Strada PizzaAlex");
		EXPECT_EQ(graph[0].phone, "07553623892");

		EXPECT_EQ(graph[1].address, "Codlea, Brasov, 9 mai bl 14");
		EXPECT_EQ(graph[1].phone, "0755632019");

		EXPECT_EQ(graph[2].address, "Brasov, strada Lunga nr 12");
		EXPECT_EQ(graph[2].phone, "0762736273");

		EXPECT_EQ(graph[3].address, "Brasov, Strada Mare nr 5");
		EXPECT_EQ(graph[3].phone, "0723626327");

		EXPECT_EQ(graph[boost::edge(0, 1, graph).first].distance, 12);
		EXPECT_EQ(graph[boost::edge(0, 1, graph).first].time, 2.5 * 12);

		EXPECT_EQ(graph[boost::edge(0, 2, graph).first].distance, 20);
		EXPECT_EQ(graph[boost::edge(0, 2, graph).first].time, 2.5 * 23);

		EXPECT_EQ(graph[boost::edge(0, 3, graph).first].distance, 30);
		EXPECT_EQ(graph[boost::edge(0, 3, graph).first].time, 2.5 * 30);

		EXPECT_EQ(graph[boost::edge(1, 2, graph).first].distance, 12);
		EXPECT_EQ(graph[boost::edge(1, 2, graph).first].time, 2.5 * 12);

		EXPECT_EQ(graph[boost::edge(1, 3, graph).first].distance, 20);
		EXPECT_EQ(graph[boost::edge(1, 3, graph).first].time, 2.5 * 20);

		EXPECT_EQ(graph[boost::edge(2, 3, graph).first].distance, 10);
		EXPECT_EQ(graph[boost::edge(2, 3, graph).first].time, 2.5 * 10);
	}
	TEST_METHOD(GraphClassTest_TestGraphEdgeCount) {
		GraphClass graphClass;
		Graph g = graphClass.generateGraph();

		int expectedEdgeCount = boost::num_vertices(g) - 1;
		int actualEdgeCount = 0;

		Graph::vertex_iterator vertexIt, vertexEnd;
		boost::tie(vertexIt, vertexEnd) = boost::vertices(g);

		for (; vertexIt != vertexEnd; ++vertexIt) {
			Graph::vertex_descriptor vd = *vertexIt;
			if (vd != 0) {
				if (boost::edge(0, vd, g).second) {
					++actualEdgeCount;
				}
			}
		}

		Assert::AreEqual(expectedEdgeCount, actualEdgeCount);
	}
	TEST_METHOD(GraphClassTest_TestGraphEdgeExistence) {
		GraphClass graphClass;
		Graph g = graphClass.generateGraph();

		Assert::IsTrue(boost::edge(0, 1, g).second);
		Assert::IsTrue(boost::edge(0, 2, g).second);
		Assert::IsTrue(boost::edge(0, 3, g).second);
		Assert::IsTrue(boost::edge(1, 2, g).second);
		Assert::IsTrue(boost::edge(1, 3, g).second);
		Assert::IsTrue(boost::edge(2, 3, g).second);
	}

};
        TEST_CLASS(showGraphTest) {
public:
	TEST_METHOD(showGraphTest_BasicTest) {
		Graph g;
		Graph::vertex_descriptor v1 = boost::add_vertex({ 1, "Codlea, Brasov, 9 mai bl 14", "0755632019" }, g);
		Graph::vertex_descriptor v2 = boost::add_vertex({ 2, "Brasov, strada Lunga nr 12", "0762736273" }, g);
		Graph::vertex_descriptor v3 = boost::add_vertex({ 3, "Brasov, Strada Mare nr 5", "0723626327" }, g);

		boost::add_edge(v1, v2, { 12, 20 }, g);
		boost::add_edge(v2, v3, { 20, 30 }, g);

		std::ostringstream oss;
		std::streambuf* cout_sbuf = std::cout.rdbuf();
		std::cout.rdbuf(oss.rdbuf());

        showGraph(g);

		std::string output = oss.str();
		std::string expected_output = "Vertex descriptor: 0\nVertex info: 1 Codlea, Brasov, 9 mai bl 14 0755632019\n"
			"Vertex descriptor: 1\nVertex info: 2 Brasov, strada Lunga nr 12 0762736273\n"
			"Vertex descriptor: 2\nVertex info: 3 Brasov, Strada Mare nr 5 0723626327\n";
		EXPECT_EQ(output, expected_output);
		std::cout.rdbuf(cout_sbuf);
	}
	TEST_METHOD(ShowGraphTest_EmptyGraph) {
		Graph g;
		std::ostringstream oss;
		std::streambuf* cout_sbuf = std::cout.rdbuf();
		std::cout.rdbuf(oss.rdbuf());

		showGraph(g);

		std::string output = oss.str();
		std::string expected_output = "";
		EXPECT_EQ(output, expected_output);
		std::cout.rdbuf(cout_sbuf);
	}
	TEST_METHOD(ShowGraphTest_OneVertexGraph) {
		Graph g;
		Graph::vertex_descriptor v1 = boost::add_vertex({ 1, "Codlea, Brasov, 9 mai bl 14", "0755632019" }, g);

		std::ostringstream oss;
		std::streambuf* cout_sbuf = std::cout.rdbuf();
		std::cout.rdbuf(oss.rdbuf());

		showGraph(g);

		std::string output = oss.str();
		std::string expected_output = "Vertex descriptor: 0\nVertex info: 1 Codlea, Brasov, 9 mai bl 14 0755632019\n";
		EXPECT_EQ(output, expected_output);
		std::cout.rdbuf(cout_sbuf);
	}
	TEST_METHOD(ShowGraphTest_TwoVertexGraph) {
		Graph g;
		Graph::vertex_descriptor v1 = boost::add_vertex({ 1, "Codlea, Brasov, 9 mai bl 14", "0755632019" }, g);
		Graph::vertex_descriptor v2 = boost::add_vertex({ 2, "Brasov, strada Lunga nr 12", "0762736273" }, g);
		boost::add_edge(v1, v2, { 12, 20 }, g);

		std::ostringstream oss;
		std::streambuf* cout_sbuf = std::cout.rdbuf();
		std::cout.rdbuf(oss.rdbuf());

		showGraph(g);

		std::string output = oss.str();
		std::string expected_output = "Vertex descriptor: 0\nVertex info: 1 Codlea, Brasov, 9 mai bl 14 0755632019\n"
			"Vertex descriptor: 1\nVertex info: 2 Brasov, strada Lunga nr 12 0762736273\n";
		EXPECT_EQ(output, expected_output);
		std::cout.rdbuf(cout_sbuf);
	}
	TEST_METHOD(ShowGraphTest_NoVertices) {
		Graph g;

		std::ostringstream oss;
		std::streambuf* cout_sbuf = std::cout.rdbuf();
		std::cout.rdbuf(oss.rdbuf());

		showGraph(g);

		std::string output = oss.str();
		std::string expected_output = "Empty graph\n";
		assert(output, expected_output);
		std::cout.rdbuf(cout_sbuf);
	}
	TEST_METHOD(ShowGraphTest_NoEdges) {
		Graph g;
		Graph::vertex_descriptor v1 = boost::add_vertex({ 1, "Codlea, Brasov, 9 mai bl 14", "0755632019" }, g);
		Graph::vertex_descriptor v2 = boost::add_vertex({ 2, "Brasov, strada Lunga nr 12", "0762736273" }, g);

		std::ostringstream oss;
		std::streambuf* cout_sbuf = std::cout.rdbuf();
		std::cout.rdbuf(oss.rdbuf());

		showGraph(g);

		std::string output = oss.str();
		std::string expected_output = "Vertex descriptor: 0\nVertex info: 1 Codlea, Brasov, 9 mai bl 14 0755632019\n"
			"Vertex descriptor: 1\nVertex info: 2 Brasov, strada Lunga nr 12 0762736273\n";
		EXPECT_EQ(output, expected_output);
		std::cout.rdbuf(cout_sbuf);
	}
};
    TEST_CLASS(goToUserTest) {
public:
	TEST_METHOD(GoToUserTest_UserExists_ReturnsDistance) {
		Graph g;
		Graph::vertex_descriptor pizzaAlex = boost::add_vertex({ 0, "Pizza Alex", "" }, g);
		Graph::vertex_descriptor user = boost::add_vertex({ 1, "Codlea, Brasov, 9 mai bl 14", "0755632019" }, g);
		boost::add_edge(pizzaAlex, user, { 10.0 }, g);


		GraphClass gc;
		float result = gc.goToUser(g, 1);

		EXPECT_FLOAT_EQ(result, 10.0);
	}
	TEST_METHOD(GoToUserTest_UserNotFound) {
		Graph g;
		Graph::vertex_descriptor pizzaAlex = boost::add_vertex({ 0, "Pizza Alex" }, g);
		Graph::vertex_descriptor user1 = boost::add_vertex({ 1, "Codlea, Brasov, 9 mai bl 14", "0755632019" }, g);
		Graph::vertex_descriptor user2 = boost::add_vertex({ 2, "Brasov, strada Lunga nr 12", "0762736273" }, g);
		boost::add_edge(pizzaAlex, user1, { 10 }, g);
		boost::add_edge(pizzaAlex, user2, { 20 }, g);
		GraphClass gc;

		float result = gc.goToUser(g, 3);

		EXPECT_EQ(result, 0);
	}
	TEST_METHOD(GoToUserTest_PizzaAlexNotFound) {
		Graph g;
		Graph::vertex_descriptor pizzaAlex = boost::add_vertex({ 0, "Pizza Alex" }, g);
		Graph::vertex_descriptor user1 = boost::add_vertex({ 1, "Codlea, Brasov, 9 mai bl 14", "0755632019" }, g);
		Graph::vertex_descriptor user2 = boost::add_vertex({ 2, "Brasov, strada Lunga nr 12", "0762736273" }, g);
		Graph::vertex_descriptor user3 = boost::add_vertex({ 5, "Brasov, Strada Mare nr 5", "0723626327" }, g);
		boost::add_edge(user1, user2, { 15 }, g);
		boost::add_edge(user2, user3, { 10 }, g);
		GraphClass gc;

		float result = gc.goToUser(g, 1);

		EXPECT_EQ(result, 0);
	}

	TEST_METHOD(GoToUserTest_UserIsDirectlyConnectedToPizzaAlex) {
		Graph g;
		Graph::vertex_descriptor pizzaAlex = boost::add_vertex({ 0, "Pizza Alex" }, g);
		Graph::vertex_descriptor user1 = boost::add_vertex({ 1, "Codlea, Brasov, 9 mai bl 14", "0755632019" }, g);
		boost::add_edge(pizzaAlex, user1, { 10 }, g);
		GraphClass gc;

		float result = gc.goToUser(g, 1);

		EXPECT_EQ(result, 10);
	}

	TEST_METHOD(GoToUserTest_UserIsIndirectlyConnectedToPizzaAlex) {
		Graph g;
		Graph::vertex_descriptor pizzaAlex = boost::add_vertex({ 0, "Pizza Alex" }, g);
		Graph::vertex_descriptor user1 = boost::add_vertex({ 1, "Codlea, Brasov, 9 mai bl 14", "0755632019" }, g);
		Graph::vertex_descriptor user2 = boost::add_vertex({ 2, "Brasov, strada Lunga nr 12", "0762736273" }, g);
		boost::add_edge(pizzaAlex, user1, { 10 }, g);
		boost::add_edge(user1, user2, { 20 }, g);
		GraphClass gc;

		float result = gc.goToUser(g, 2);

		assert(result, 30);
	}
	TEST_METHOD(GoToUserTest_UserNotFound_ReturnsZero) {
		Graph g;
		Graph::vertex_descriptor pizzaAlex = boost::add_vertex({ 0, "Pizza Alex" }, g);
		Graph::vertex_descriptor user1 = boost::add_vertex({ 1, "Codlea, Brasov, 9 mai bl 14", "0755632019" }, g);
		boost::add_edge(pizzaAlex, user1, { 10 }, g);
		GraphClass gc;

		float result = gc.goToUser(g, 3);

		Assert::AreEqual(result, 0.0f);
	}
	TEST_METHOD(GoToUserTest_PizzaAlexNotFound_ReturnsZero) {
		Graph g;
		Graph::vertex_descriptor pizzaAlex = boost::add_vertex({ 0, "Pizza Alex" }, g);
		Graph::vertex_descriptor user1 = boost::add_vertex({ 1, "Codlea, Brasov, 9 mai bl 14", "0755632019" }, g);
		Graph::vertex_descriptor user2 = boost::add_vertex({ 2, "Brasov, strada Lunga nr 12", "0762736273" }, g);
		Graph::vertex_descriptor user3 = boost::add_vertex({ 5, "Brasov, Strada Mare nr 5", "0723626327" }, g);
		boost::add_edge(user1, user2, { 15 }, g);
		boost::add_edge(user2, user3, { 10 }, g);
		GraphClass gc;

		float result = gc.goToUser(g, 1);

		Assert::AreEqual(result, 0.0f);
	}
	TEST_METHOD(GoToUserTest_UserIsDirectlyConnectedToPizzaAlex_ReturnsDistance) {
		Graph g;
		Graph::vertex_descriptor pizzaAlex = boost::add_vertex({ 0, "Pizza Alex" }, g);
		Graph::vertex_descriptor user1 = boost::add_vertex({ 1, "Codlea, Brasov, 9 mai bl 14", "0755632019" }, g);
		boost::add_edge(pizzaAlex, user1, { 10 }, g);
		GraphClass gc;

		float result = gc.goToUser(g, 1);

		Assert::AreEqual(result, 10.0f);
	}
	TEST_METHOD(GoToUserTest_UserIsIndirectlyConnectedToPizzaAlex_ReturnsDistance) {
		Graph g;
		Graph::vertex_descriptor pizzaAlex = boost::add_vertex({ 0, "Pizza Alex" }, g);
		Graph::vertex_descriptor user1 = boost::add_vertex({ 1, "Codlea, Brasov, 9 mai bl 14", "0755632019" }, g);
		Graph::vertex_descriptor user2 = boost::add_vertex({ 2, "Brasov, strada Lunga nr 12", "0762736273" }, g);
		boost::add_edge(pizzaAlex, user1, { 10 }, g);
		boost::add_edge(user1, user2, { 20 }, g);
		GraphClass gc;

		float result = gc.goToUser(g, 2);

		Assert::AreEqual(result, 30.0f);
	}
};


	//ThreadOvens.cpp
	TEST_CLASS(VerifyEmptyOvensTest) {
public:
	TEST_METHOD(ThreadOvensTest_verifyEmptyOvensTestNoOvensAvailable) {
		for (int i = 0; i < 5; i++) {
			oven[i].setAvailable(false);
		}
		assert(threadOvens.verifyEmptyOvens() == 0);
	}
	TEST_METHOD(ThreadOvensTest_verifyEmptyOvenstestAllOvensAvailable) {
		assert(threadOvens.verifyEmptyOvens() == 5);
	}
	TEST_METHOD(ThreadOvensTest_verifyEmptyOvensTestSomeOvensAvailable) {
		oven[1].setAvailable(false);
		oven[3].setAvailable(false);
		assert(threadOvens.verifyEmptyOvens() == 3);
	}
	};
	TEST_CLASS(processingOrdersTest) {
	public:
	TEST_METHOD(ThreadOvensTest_ConnectionNull_NoOvenUsed) {
			sql::Connection* con = nullptr;
			ThreadOvens threadOvens;

			threadOvens.ovens(con);

			for (int i = 0; i < 5; i++) {
				Assert::IsTrue(oven[i].isAvailable());
				Assert::IsTrue(oven[i].isDoneBaking());
			}
		}
	TEST_METHOD(ThreadOvensTest_SomeOvensAvailable) {
			ThreadOvens ovens;
			OrderClass orderWithItems;
			sql::Connection* con = nullptr;

			for (int i = 0; i < 5; i++) {
				oven[i].setAvailable(true);
			}

			EXPECT_NO_THROW(ovens.processingOrder(orderWithItems, con));

			for (int i = 0; i < 5; i++) {
				Assert::IsTrue(oven[i].isDoneBaking());
			}
		}
	TEST_METHOD(ThreadOvensTest_EmptyOrderList_AllOvensBusy) {
			ThreadOvens ovens;
			OrderClass orderWithItems;
			sql::Connection* con = nullptr;

			std::thread otherThread([&ovens]() {
				OrderClass otherOrder;
				sql::Connection* otherCon = nullptr;
				ovens.processingOrder(otherOrder, otherCon);
				});

			assert(ovens.processingOrder(orderWithItems, con));
			otherThread.join();

		}
	TEST_METHOD(ThreadOvensTest_EmptyOrderList_InvalidDatabaseConnection) {
			OrderClass orderWithItems;
			sql::Connection* invalidCon = nullptr;

			assert(threadOvens.processingOrder(orderWithItems, invalidCon));
		}

	TEST_METHOD(ThreadOvensTest_EmptyOrderList_ValidDatabaseConnection) {
			OrderClass orderWithItems;
			sql::Connection* validCon = nullptr;

			assert(threadOvens.processingOrder(orderWithItems, validCon));
		}
	};
	TEST_CLASS(ovensTest) {
	public:
	TEST_METHOD(ThreadovensTest_NotClearedIfNotDoneBaking) {
			OvenClass* oven = new OvenClass[5];
			sql::Connection* con = nullptr;
			oven[2].setAvailable(false);
			oven[2].setDoneBaking(false);

			long currentTime = std::chrono::system_clock::now().time_since_epoch().count();
			double currentTime_seconds = currentTime / static_cast<double>(std::chrono::system_clock::period::den);
			double ovenTime_seconds = oven[2].getTime() / static_cast<double>(std::chrono::system_clock::period::den);
			double prepareTime_milliseconds = oven[2].getItem().getPrepareTime() / static_cast<double>(std::chrono::system_clock::period::den);
			double prep_seconds = prepareTime_milliseconds * 10000000;
			oven[2].setTime();

			oven[2].setQuantity(oven[2].getItem().getQuantity());

			ThreadOvens::ovens(con);

			EXPECT_FALSE(oven[2].isAvailable());
			EXPECT_FALSE(oven[2].isDoneBaking());
			EXPECT_EQ(oven[2].getQuantity(), oven[2].getItem().getQuantity());
		}
	};

	//ThreadDelivery.cpp
	TEST_CLASS(checkFreeDeliverersTest) {
public:
	TEST_METHOD(ThreadDeliverersTest_checkFreeDeliverersTestNoDelivererAvailable) {
		for (int i = 0; i < 5; i++) {
			deliverer[i].setDelivererAvailable(false);
		}
		assert(threadDelivery.checkFreeDeliverers == 0);
	}
	TEST_METHOD(ThreadDeliverersTest_checkFreeDeliverersTestAllDelivererAvailable) {
		assert(threadDelivery.checkFreeDeliverers() == 5);
	}
	TEST_METHOD(ThreadDeliverersTest_checkFreeDeliverersTestSomeOvensAvailable) {
		deliverer[2].setDelivererAvailable(false);
		deliverer[4].setDelivererAvailable(false);
		assert(threadDelivery.checkFreeDeliverers() == 3);
	}
	};
}
