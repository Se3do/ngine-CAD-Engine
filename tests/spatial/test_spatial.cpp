#include <ngine/core/types.hpp>
#include <ngine/spatial/quadtree.hpp>

#include <gtest/gtest.h>

using namespace ngine;

TEST(QuadtreeTest, InsertAndQuery) {
    Quadtree qt;
    qt.insert(1, BoundingBox(0, 0, 1, 1));
    qt.insert(2, BoundingBox(5, 5, 6, 6));

    EXPECT_EQ(qt.size(), 2U);

    const auto results = qt.query(BoundingBox(-1, -1, 2, 2));
    ASSERT_EQ(results.size(), 1U);
    EXPECT_EQ(results.at(0), 1U);
}

TEST(QuadtreeTest, QueryRadius) {
    Quadtree qt;
    qt.insert(1, BoundingBox(0, 0, 1, 1));
    qt.insert(2, BoundingBox(100, 100, 101, 101));

    const auto results = qt.query_radius(Point(0.5, 0.5), 5.0);
    ASSERT_EQ(results.size(), 1U);
    EXPECT_EQ(results.at(0), 1U);
}

TEST(QuadtreeTest, Remove) {
    Quadtree qt;
    qt.insert(1, BoundingBox(0, 0, 1, 1));
    qt.insert(2, BoundingBox(2, 2, 3, 3));

    qt.remove(1);
    EXPECT_EQ(qt.size(), 1U);

    const auto results = qt.query(BoundingBox(-1, -1, 5, 5));
    ASSERT_EQ(results.size(), 1U);
    EXPECT_EQ(results.at(0), 2U);
}

TEST(QuadtreeTest, Update) {
    Quadtree qt;
    qt.insert(1, BoundingBox(0, 0, 1, 1));

    qt.update(1, BoundingBox(50, 50, 51, 51));

    auto results = qt.query(BoundingBox(-1, -1, 2, 2));
    EXPECT_TRUE(results.empty());

    results = qt.query(BoundingBox(49, 49, 52, 52));
    ASSERT_EQ(results.size(), 1U);
}

TEST(QuadtreeTest, Clear) {
    Quadtree qt;
    qt.insert(1, BoundingBox(0, 0, 1, 1));
    qt.insert(2, BoundingBox(2, 2, 3, 3));

    qt.clear();
    EXPECT_EQ(qt.size(), 0U);
}

TEST(QuadtreeTest, ManyInserts) {
    Quadtree qt;
    for (EntityId i = 1; i <= 100; ++i) {
        const Real x = static_cast<Real>(i) * 10.0;
        qt.insert(i, BoundingBox(x, x, x + 1, x + 1));
    }
    EXPECT_EQ(qt.size(), 100U);

    const auto results = qt.query(BoundingBox(0, 0, 1000, 1000));
    EXPECT_EQ(results.size(), 100U);
}

TEST(BoundingBoxTest, Contains) {
    const BoundingBox bb(0, 0, 10, 10);
    EXPECT_TRUE(bb.contains(Point(5, 5)));
    EXPECT_TRUE(bb.contains(Point(0, 0)));
    EXPECT_FALSE(bb.contains(Point(11, 5)));
}

TEST(BoundingBoxTest, Intersects) {
    const BoundingBox a(0, 0, 10, 10);
    const BoundingBox b(5, 5, 15, 15);
    const BoundingBox c(20, 20, 30, 30);

    EXPECT_TRUE(a.intersects(b));
    EXPECT_FALSE(a.intersects(c));
}

TEST(BoundingBoxTest, ContainsBox) {
    const BoundingBox outer(0, 0, 10, 10);
    const BoundingBox inner(2, 2, 8, 8);
    const BoundingBox partial(5, 5, 15, 15);

    EXPECT_TRUE(outer.contains_box(inner));
    EXPECT_FALSE(outer.contains_box(partial));
}
