#include "SessionManager.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::Return;
using ::testing::_;

// ==========================================
// PART 1: THE MOCKS & STUBS
// ==========================================

class MockUserRepository : public IUserRepository {
public:
    MOCK_METHOD(bool, checkCredentials, (const std::string&, const std::string&), (override));
};

class MockTwoFactorAuth : public ITwoFactorAuth {
public:
    MOCK_METHOD(void, generateCode, (const std::string&), (override));
    MOCK_METHOD(bool, validateCode, (const std::string&, const std::string&), (override));
};

// ==========================================
// PART 2: THE TEST SUITE SETUP
// ==========================================

class SessionManagerTest : public ::testing::Test {
protected:
    MockUserRepository mockRepo;
    MockTwoFactorAuth mockTFA;
    SessionManager* session;

    void SetUp() override {
        session = new SessionManager(&mockRepo, &mockTFA);
    }

    void TearDown() override {
        delete session;
    }
};

// ==========================================
// PART 3: THE UNIT TESTS
// ==========================================

// Test 1: STUB for Invalid Login
TEST_F(SessionManagerTest, LoginFailsWithBadCredentials) {
    EXPECT_CALL(mockRepo, checkCredentials("JohnDoe", "wrongpass"))
        .WillOnce(Return(false));

    LoginState result = session->login("JohnDoe", "wrongpass");
    EXPECT_EQ(result, LoginState::FAILURE);
}

// Test 2: MOCK for 2FA Generation Behavior
TEST_F(SessionManagerTest, LoginTriggers2FAGenerationOnSuccess) {
    EXPECT_CALL(mockRepo, checkCredentials("JohnDoe", "goodpass"))
        .WillOnce(Return(true));

    EXPECT_CALL(mockTFA, generateCode("JohnDoe"))
        .Times(1);

    LoginState result = session->login("JohnDoe", "goodpass");
    EXPECT_EQ(result, LoginState::PENDING_2FA);
}

// Test 3: STUB for Successful 2FA Verification
TEST_F(SessionManagerTest, Verify2FASucceedsWithValidCode) {
    EXPECT_CALL(mockTFA, validateCode("JohnDoe", "123456"))
        .WillOnce(Return(true));

    LoginState result = session->verify2FA("JohnDoe", "123456");
    EXPECT_EQ(result, LoginState::SUCCESS);
}

// Test 4: STUB for Failed 2FA Verification
TEST_F(SessionManagerTest, Verify2FAFailsWithInvalidCode) {
    EXPECT_CALL(mockTFA, validateCode("JohnDoe", "000000"))
        .WillOnce(Return(false));

    LoginState result = session->verify2FA("JohnDoe", "000000");
    EXPECT_EQ(result, LoginState::FAILURE);
}