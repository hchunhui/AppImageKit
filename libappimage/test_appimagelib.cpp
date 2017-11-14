#include "libappimage.h"

#include <sys/stat.h>
#include <sys/types.h>

#include <string>
#include <gtest/gtest.h>

namespace AppImageTests
{

class AppImageTest : public testing::Test
{
  protected:
    char tests_dir[250];
    bool tests_dir_created = false;
    std::string test_file_content;

    virtual void SetUp()
    {
        test_file_content  = "Hello World\n";
        createTestsDir();
    }

    virtual void TearDown()
    {
        removeTestsDir();
    }

    inline void createTestsDir()
    {
        sprintf(tests_dir, "/tmp/appimagelib_tests_dir_%d/", rand());

        int result = mkdir(tests_dir, 0700);
        tests_dir_created = !result;
    }

    inline void removeTestsDir()
    {
        rmdir(tests_dir);
    }

    std::string build_test_file_path(std::string name)
    {
        return tests_dir + name;
    }

    void mk_file(std::string path)
    {
        
        g_file_set_contents(path.c_str(),
                            test_file_content.c_str(),
                            test_file_content.size(),
                            0);
    }

    void rm_file(std::string path)
    {
        g_remove(path.c_str());
    }
};

TEST_F(AppImageTest, set_executable)
{
    std::string path = AppImageTest::build_test_file_path("tmp_file");
    mk_file(path);

    set_executable(path.c_str(), 0);

    int isExecutable = g_file_test(path.c_str(), G_FILE_TEST_IS_EXECUTABLE);
    ASSERT_TRUE(isExecutable);
    
    rm_file(path);
}

TEST_F(AppImageTest, replace_str) {
    std::string s1 = "hello sad world";
    std::string s2 = "hello world";
    
    gchar *result = replace_str(s1.c_str(), " sad", "");
    int res = g_strcmp0(s2.c_str(), result);
    ASSERT_TRUE(res == 0);
}

TEST_F(AppImageTest, get_md5)
{
    // generated using md5sum
    std::string expected = "f0ef7081e1539ac00ef5b761b4fb01b3";
    
    gchar * sum = get_md5(test_file_content.c_str());
    
    std::cout << sum;
    int res = g_strcmp0(expected.c_str(), sum);
    ASSERT_TRUE(res == 0);
}

} // namespace

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}