#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define M 1000000007
#define fio                           \
    ios_base::sync_with_stdio(false); \
    cin.tie(NULL);
#define d(x) cout << #x << " " << x << "\n";
#define min(x1, x2) (x1 > x2 ? x2 : x1)
#define max(x1, x2) (x1 < x2 ? x2 : x1)
#define min3(x1, x2, x3) (x3 > min(x1, x2) ? min(x2, x1) : x3)
#define max3(x1, x2, x3) (x3 < max(x1, x2) ? max(x1, x2) : x3)
#define ll long long int
#define ul unsigned long long int
#define p pair<ll, ll>
#define ld long double
#define dv(v)                            \
    cerr << #v << "  ";                  \
    for (int i = 0; i < (v).size(); i++) \
        cerr << v[i] << " ";             \
    cerr << "\n";
#define inf LONG_LONG_MAX
#define mp(x, y) make_pair(x, y)
#define oset tree<p, null_type, less<p>, rb_tree_tag, tree_order_statistics_node_update>
using namespace __gnu_pbds;
using namespace std;

class User;
class Blog;

class User
{
public:
    User(int, string);
    string name;
    int id;
    vector<Blog *> blogs;
    vector<User *> following;
    Blog *top_rated_blog;
    void follow_user(User *);
    vector<Blog *> get_feed();
    void like_blog(Blog *);
    int get_rank();
};

class Blog
{
public:
    static set<p> top_10_blogs; // {likes,blog_id}
    static oset user_ranks;     // (most_liked_blog_likes,user_id)
    static void get_top_10_most_liked_blogs();
    int likes;
    int id;
    User *writer;
    Blog(int, User *);
};

set<p> Blog::top_10_blogs = {};
oset Blog::user_ranks = {};

void Blog::get_top_10_most_liked_blogs()
{
    int k = 1;
    for (auto it = top_10_blogs.rbegin(); it != top_10_blogs.rend(); ++it)
    {
        cout << k << ". " << it->second << " "
             << " likes=" << it->first << endl;
        ++k;
    }
}

User::User(int user_id, string username)
{
    id = user_id;
    name = username;
    Blog::user_ranks.insert(mp(0, id));
    top_rated_blog = NULL;
}

Blog::Blog(int blog_id, User *writer)
{
    id = blog_id;
    likes = 0;
    this->writer = writer;
    if (top_10_blogs.size() < 10)
    {
        top_10_blogs.insert({0, blog_id});
    }
    writer->blogs.push_back(this);
    if (writer->top_rated_blog == NULL)
    {
        writer->top_rated_blog = this;
    }
}

void User::follow_user(User *to_follow)
{
    following.push_back(to_follow);
}

vector<Blog *> User::get_feed()
{
    vector<Blog *> feed;
    for (auto user_following : following)
    {
        for (auto blog : user_following->blogs)
        {
            feed.push_back(blog);
        }
    }
    return feed;
}

void User::like_blog(Blog *blog)
{
    User *blog_writer = blog->writer;
    if (blog_writer->top_rated_blog->likes < blog->likes + 1)
    {
        Blog::user_ranks.erase({blog_writer->top_rated_blog->likes, blog_writer->id});
        blog_writer->top_rated_blog = blog;
        Blog::user_ranks.insert({blog_writer->top_rated_blog->likes + 1, blog_writer->id});
    }
    auto it = Blog::top_10_blogs.find({blog->likes, blog->id});
    if (it != Blog::top_10_blogs.end())
    {
        Blog::top_10_blogs.erase(it);
        Blog::top_10_blogs.insert({blog->likes + 1, blog->id});
    }
    else
    {
        auto it = Blog::top_10_blogs.begin();
        if (it->first < blog->likes + 1)
        {
            Blog::top_10_blogs.erase(it);
            Blog::top_10_blogs.insert({blog->likes + 1, blog->id});
        }
    }
    blog->likes++;
}

int User::get_rank()
{
    int most_likes = 0;
    if (top_rated_blog != NULL)
    {
        most_likes = top_rated_blog->likes;
    }
    return Blog::user_ranks.size() - Blog::user_ranks.order_of_key({most_likes, id});
}

int main()
{
    fio;
    ll t, n, x, y, z, k;
    User u1(100, "sidak");
    User u2(200, "influencer1");
    User u3(300, "influencer2");

    vector<Blog *> all_blogs;
    for (int i = 0; i < 20; ++i)
    {
        if (i > 9)
        {
            Blog b(i, &u3);
        }
        else
        {
            Blog b(i, &u2);
        }
    }

    u1.follow_user(&u2);
    u1.follow_user(&u3);

    vector<Blog *> feed_u1 = u1.get_feed();
    d(feed_u1.size());
    srand(time(0));
    for (int i = 0; i < 100; ++i)
    {
        int j = rand() % 20;
        u1.like_blog(feed_u1[j]);
    }
    Blog::get_top_10_most_liked_blogs();
    d(u1.get_rank());
    d(u2.get_rank());
    return 0;
}