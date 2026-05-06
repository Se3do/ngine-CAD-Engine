#include <ngine/spatial/quadtree.hpp>

#include <gtest/gtest.h>

using namespace ngine;

TEST(QuadtreeTest, InsertAndQuery) {
    Quadtree qt;
    qt.insert(1, BoundingBox(0, 0, 1, 1));
    qt.insert(2, BoundingBox(5, 5, 6, 6));

    EXPECT_EQ(qt.size(), 2u);

    auto results = qt.query(BoundingBox(-1, -1, 2, 2));
    ASSERT_EQ(results.size(), 1u);
    EXPECT_EQ(results[0], 1u);
}

TEST(QuadtreeTest, QueryRadius) {
    Quadtree qt;
    qt.insert(1, BoundingBox(0, 0, 1, 1));
    qt.insert(2, BoundingBox(100, 100, 101, 101));

    auto results = qt.query_radius(Point(0.5, 0.5), 5.0);
    ASSERT_EQ(results.size(), 1u);
    EXPECT_EQ(results[0], 1u);
}

TEST(QuadtreeTest, Remove) {
    Quadtree qt;
    qt.insert(1, BoundingBox(0, 0, 1, 1));
    qt.insert(2, BoundingBox(2, 2, 3, 3));

    qt.remove(1);
    EXPECT_EQ(qt.size(), 1u);

    auto results = qt.query(BoundingBox(-1, -1, 5, 5));
    ASSERT_EQ(results.size(), 1u);
    EXPECT_EQ(results[0], 2u);
}

TEST(QuadtreeTest, Update) {
    Quadtree qt;
    qt.insert(1, BoundingBox(0, 0, 1, 1));

    qt.update(1, BoundingBox(50, 50, 51, 51));

    auto results = qt.query(BoundingBox(-1, -1, 2, 2));
    EXPECT_TRUE(results.empty());

    results = qt.query(BoundingBox(49, 49, 52, 52));
    ASSERT_EQ(results.size(), 1u);
}

TEST(QuadtreeTest, Clear) {
    Quadtree qt;
    qt.insert(1, BoundingBox(0, 0, 1, 1));
    qt.insert(2, BoundingBox(2, 2, 3, 3));

    qt.clear();
    EXPECT_EQ(qt.size(), 0u);
}

TEST(QuadtreeTest, ManyInserts) {
    Quadtree qt;
    for (EntityId i = 1; i <= 100; ++i) {
        Real x = static_cast<Real>(i) * 10.0;
        qt.insert(i, BoundingBox(x, x, x + 1, x + 1));
    }
    EXPECT_EQ(qt.size(), 100u);

    auto results = qt.query(BoundingBox(0, 0, 1000, 1000));
    EXPECT_EQ(results.size(), 100u);
}

TEST(BoundingBoxTest, Contains) {
    BoundingBox bb(0, 0, 10, 10);
    EXPECT_TRUE(bb.contains(Point(5, 5)));
    EXPECT_TRUE(bb.contains(Point(0, 0)));
    EXPECT_FALSE(bb.contains(Point(11, 5)));
}

TEST(BoundingBoxTest, Intersects) {
    BoundingBox a(0, 0, 10, 10);
    BoundingBox b(5, 5, 15, 15);
    BoundingBox c(20, 20, 30, 30);

    EXPECT_TRUE(a.intersects(b));
    EXPECT_FALSE(a.intersects(c));
}

TEST(BoundingBoxTest, ContainsBox) {
    BoundingBox outer(0, 0, 10, 10);
    BoundingBox inner(2, 2, 8, 8);
    BoundingBox partial(5, 5, 15, 15);

    EXPECT_TRUE(outer.contains_box(inner));
    EXPECT_FALSE(outer.contains_box(partial));
}
