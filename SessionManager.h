#include <string>

// --- DEPENDENCIES (Interfaces) ---
class IUserRepository {
public:
    virtual ~IUserRepository() = default;
    virtual bool checkCredentials(const std::string& username, const std::string& password) = 0;
};

class ITwoFactorAuth {
public:
    virtual ~ITwoFactorAuth() = default;
    virtual void generateCode(const std::string& username) = 0;
    virtual bool validateCode(const std::string& username, const std::string& code) = 0;
};

// --- SYSTEM UNDER TEST ---
enum class LoginState { SUCCESS, FAILURE, PENDING_2FA };

class SessionManager {
private:
    IUserRepository* userRepo;
    ITwoFactorAuth* tfaService;

public:
    SessionManager(IUserRepository* repo, ITwoFactorAuth* tfa) 
        : userRepo(repo), tfaService(tfa) {}

    // Business Logic: If credentials are good, trigger 2FA. Otherwise, fail.
    LoginState login(const std::string& username, const std::string& password) {
        if (userRepo->checkCredentials(username, password)) {
            tfaService->generateCode(username); // Side-effect we want to mock
            return LoginState::PENDING_2FA;
        }
        return LoginState::FAILURE;
    }

    LoginState verify2FA(const std::string& username, const std::string& code) {
        if (tfaService->validateCode(username, code)) {
            return LoginState::SUCCESS;
        }
        return LoginState::FAILURE;
    }
};