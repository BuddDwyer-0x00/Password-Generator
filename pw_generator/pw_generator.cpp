#include <Windows.h>
#include <iostream>
#include <random>
#include <string>
#include "niggercrypt.h"

using namespace std;

std::string random_string(std::string::size_type length)
{
    static auto& chrs = "0123456789!?;,:._-abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    thread_local static std::mt19937 rg{ std::random_device{}() };
    thread_local static std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chrs) - 2);

    std::string random;

    random.reserve(length);

    while (length--)
        random += chrs[pick(rg)];

    return random;
}

LONG WINAPI UnhandledExcepFilter(PEXCEPTION_POINTERS pExcepPointers)
{
    // Restore old UnhandledExceptionFilter
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)
        pExcepPointers->ContextRecord->Eax);

    // Skip the exception code
    pExcepPointers->ContextRecord->Eip += 2;

    return EXCEPTION_CONTINUE_EXECUTION;
}

void DebugSelf()
{
    HANDLE hProcess = NULL;
    DEBUG_EVENT de;
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&si, sizeof(STARTUPINFO));
    ZeroMemory(&de, sizeof(DEBUG_EVENT));

    GetStartupInfo(&si);

    // Create the copy of ourself
    CreateProcess(NULL, GetCommandLine(), NULL, NULL, FALSE,
        DEBUG_PROCESS, NULL, NULL, &si, &pi);

    // Continue execution
    ContinueDebugEvent(pi.dwProcessId, pi.dwThreadId, DBG_CONTINUE);

    // Wait for an event
    WaitForDebugEvent(&de, INFINITE);
}

void __stdcall security(LPVOID lparam)
{
    OutputDebugString(TEXT("%s%s%s%s%s%s%s%s%s%s%s")
        TEXT("%s%s%s%s%s%s%s%s%s%s%s%s%s")
        TEXT("%s%s%s%s%s%s%s%s%s%s%s%s%s")
        TEXT("%s%s%s%s%s%s%s%s%s%s%s%s%s"));

    SetUnhandledExceptionFilter(UnhandledExcepFilter);

    if (IsDebuggerPresent()) {
        exit(0);
    }

    DebugSelf();

    printf(NiggerCrypt("Security done. \n"));
}

void main()
{
    // initializing
    security(0);
    system(NiggerCrypt("@echo off"));
	system(NiggerCrypt("color a"));
    SetConsoleTitleA(NiggerCrypt("PW Generator"));
    HWND hwnd = GetConsoleWindow();
    int custom_size;

    std::string strenght;
    printf(NiggerCrypt("Password length: "));
    cin >> custom_size;
    cout << endl;

    // checking password, ghetto and stupid shit
    if (custom_size <= 5) {
        printf(NiggerCrypt("Too short."));
        Sleep(5000);
        exit(0);
    }
    else if (custom_size >= 255) {
        printf(NiggerCrypt("Too long."));
        Sleep(5000);
        exit(0);
    }

    if (custom_size < 10) {
        strenght = (NiggerCrypt("Weak."));
    }
    else if (custom_size >= 10 && custom_size <= 15) {
        strenght = (NiggerCrypt("Medium."));
    }
    else if (custom_size > 15) {
        strenght = (NiggerCrypt("Strong."));
    }

    std::string bitch = random_string(custom_size);

    SetConsoleTitleA(bitch.c_str());

    // copy to clipbboard
    OpenClipboard(hwnd);
    EmptyClipboard();
    HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, bitch.size() + 1);
    memcpy(GlobalLock(hg), bitch.c_str(), bitch.size() + 1);
    GlobalUnlock(hg);
    SetClipboardData(CF_TEXT, hg);
    GlobalFree(hg);
    CloseClipboard();
    
    // text shit
    cout << "Random password: " << bitch << endl;
    cout << "Copied password to clipboard" << endl;
    cout << "Your password is ranked as: " << strenght << endl;

    Sleep(5000);
    exit(0);
}
