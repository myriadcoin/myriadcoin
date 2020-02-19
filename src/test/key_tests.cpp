// Copyright (c) 2012-2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <key.h>

#include <key_io.h>
#include <script/script.h>
#include <uint256.h>
#include <util/system.h>
#include <util/strencodings.h>
#include <test/test_bitcoin.h>

#include <string>
#include <vector>

#include <boost/test/unit_test.hpp>

static const std::string strSecret1 = "6z8tDFZ9cJy8CWVAZiHawKM7DbhDdCpnQEyLDNUwMz4LBvqDSzb";
static const std::string strSecret2 = "6zLeARdm3LVHKX1QEx6B1Ks936mWrZNfRJydmpXwhYEqsgg3YGW";
static const std::string strSecret1C = "TRBo35EtzrJimo9KDPECnjykSc8WioBuPafdobpJwcqhgpdQaQCf";
static const std::string strSecret2C = "TS5h2bjFPK1q2jSmaa9qASGJUGTUmjtcaJLyJpoaRvt6iRvg2k73";
static const std::string addr1 = "MX7aKiuxRxQ4UnaseF41GzxBuWqHJ2GUwy";
static const std::string addr2 = "MEBPbnApwv5fetLH7eVvaxPNUY8WV9d8jD";
static const std::string addr1C = "MBeR1CLYqGPoonVC18AAywEuho3MeGFSwP";
static const std::string addr2C = "MTpDtfFCDBi9nJ3kmZJPavhw65j62D3p86";

static const std::string strAddressBad = "1HV9Lc3sNHZxwj4Zk6fB38tEmBryq2cBiF";


BOOST_FIXTURE_TEST_SUITE(key_tests, BasicTestingSetup)

BOOST_AUTO_TEST_CASE(key_test1)
{
    CKey key1  = DecodeSecret(strSecret1);
    BOOST_CHECK(key1.IsValid() && !key1.IsCompressed());
    CKey key2  = DecodeSecret(strSecret2);
    BOOST_CHECK(key2.IsValid() && !key2.IsCompressed());
    CKey key1C = DecodeSecret(strSecret1C);
    BOOST_CHECK(key1C.IsValid() && key1C.IsCompressed());
    CKey key2C = DecodeSecret(strSecret2C);
    BOOST_CHECK(key2C.IsValid() && key2C.IsCompressed());
    CKey bad_key = DecodeSecret(strAddressBad);
    BOOST_CHECK(!bad_key.IsValid());

    CPubKey pubkey1  = key1. GetPubKey();
    CPubKey pubkey2  = key2. GetPubKey();
    CPubKey pubkey1C = key1C.GetPubKey();
    CPubKey pubkey2C = key2C.GetPubKey();

    BOOST_CHECK(key1.VerifyPubKey(pubkey1));
    BOOST_CHECK(!key1.VerifyPubKey(pubkey1C));
    BOOST_CHECK(!key1.VerifyPubKey(pubkey2));
    BOOST_CHECK(!key1.VerifyPubKey(pubkey2C));

    BOOST_CHECK(!key1C.VerifyPubKey(pubkey1));
    BOOST_CHECK(key1C.VerifyPubKey(pubkey1C));
    BOOST_CHECK(!key1C.VerifyPubKey(pubkey2));
    BOOST_CHECK(!key1C.VerifyPubKey(pubkey2C));

    BOOST_CHECK(!key2.VerifyPubKey(pubkey1));
    BOOST_CHECK(!key2.VerifyPubKey(pubkey1C));
    BOOST_CHECK(key2.VerifyPubKey(pubkey2));
    BOOST_CHECK(!key2.VerifyPubKey(pubkey2C));

    BOOST_CHECK(!key2C.VerifyPubKey(pubkey1));
    BOOST_CHECK(!key2C.VerifyPubKey(pubkey1C));
    BOOST_CHECK(!key2C.VerifyPubKey(pubkey2));
    BOOST_CHECK(key2C.VerifyPubKey(pubkey2C));

    BOOST_CHECK(DecodeDestination(addr1)  == CTxDestination(pubkey1.GetID()));
    BOOST_CHECK(DecodeDestination(addr2)  == CTxDestination(pubkey2.GetID()));
    BOOST_CHECK(DecodeDestination(addr1C) == CTxDestination(pubkey1C.GetID()));
    BOOST_CHECK(DecodeDestination(addr2C) == CTxDestination(pubkey2C.GetID()));

    for (int n=0; n<16; n++)
    {
        std::string strMsg = strprintf("Very secret message %i: 11", n);
        uint256 hashMsg = Hash(strMsg.begin(), strMsg.end());

        // normal signatures

        std::vector<unsigned char> sign1, sign2, sign1C, sign2C;

        BOOST_CHECK(key1.Sign (hashMsg, sign1));
        BOOST_CHECK(key2.Sign (hashMsg, sign2));
        BOOST_CHECK(key1C.Sign(hashMsg, sign1C));
        BOOST_CHECK(key2C.Sign(hashMsg, sign2C));

        BOOST_CHECK( pubkey1.Verify(hashMsg, sign1));
        BOOST_CHECK(!pubkey1.Verify(hashMsg, sign2));
        BOOST_CHECK( pubkey1.Verify(hashMsg, sign1C));
        BOOST_CHECK(!pubkey1.Verify(hashMsg, sign2C));

        BOOST_CHECK(!pubkey2.Verify(hashMsg, sign1));
        BOOST_CHECK( pubkey2.Verify(hashMsg, sign2));
        BOOST_CHECK(!pubkey2.Verify(hashMsg, sign1C));
        BOOST_CHECK( pubkey2.Verify(hashMsg, sign2C));

        BOOST_CHECK( pubkey1C.Verify(hashMsg, sign1));
        BOOST_CHECK(!pubkey1C.Verify(hashMsg, sign2));
        BOOST_CHECK( pubkey1C.Verify(hashMsg, sign1C));
        BOOST_CHECK(!pubkey1C.Verify(hashMsg, sign2C));

        BOOST_CHECK(!pubkey2C.Verify(hashMsg, sign1));
        BOOST_CHECK( pubkey2C.Verify(hashMsg, sign2));
        BOOST_CHECK(!pubkey2C.Verify(hashMsg, sign1C));
        BOOST_CHECK( pubkey2C.Verify(hashMsg, sign2C));

        // compact signatures (with key recovery)

        std::vector<unsigned char> csign1, csign2, csign1C, csign2C;

        BOOST_CHECK(key1.SignCompact (hashMsg, csign1));
        BOOST_CHECK(key2.SignCompact (hashMsg, csign2));
        BOOST_CHECK(key1C.SignCompact(hashMsg, csign1C));
        BOOST_CHECK(key2C.SignCompact(hashMsg, csign2C));

        CPubKey rkey1, rkey2, rkey1C, rkey2C;

        BOOST_CHECK(rkey1.RecoverCompact (hashMsg, csign1));
        BOOST_CHECK(rkey2.RecoverCompact (hashMsg, csign2));
        BOOST_CHECK(rkey1C.RecoverCompact(hashMsg, csign1C));
        BOOST_CHECK(rkey2C.RecoverCompact(hashMsg, csign2C));

        BOOST_CHECK(rkey1  == pubkey1);
        BOOST_CHECK(rkey2  == pubkey2);
        BOOST_CHECK(rkey1C == pubkey1C);
        BOOST_CHECK(rkey2C == pubkey2C);
    }

    // test deterministic signing

    std::vector<unsigned char> detsig, detsigc;
    std::string strMsg = "Very deterministic message";
    uint256 hashMsg = Hash(strMsg.begin(), strMsg.end());
    BOOST_CHECK(key1.Sign(hashMsg, detsig));
    BOOST_CHECK(key1C.Sign(hashMsg, detsigc));
    BOOST_CHECK(detsig == detsigc);
    BOOST_CHECK(detsig == ParseHex("304402207a0500682006ecf3d27ccf0bfbcbf0bb2b6e29d1e6a277c9c6dcc838969e8bf50220752f4ff7a37ef47765b92b799b935a8cb0b5e5bda2e398b457ddb6ac5e8bfee9"));
    BOOST_CHECK(key2.Sign(hashMsg, detsig));
    BOOST_CHECK(key2C.Sign(hashMsg, detsigc));
    BOOST_CHECK(detsig == detsigc);
    BOOST_CHECK(detsig == ParseHex("3044022039fe3b85606b5d82438cd59adb60ec455df718c5e408a6c78c97e9de76674e970220574b822f8e38406cc55828a28d5d1dd870aa5142c116912f302ca7f3f266deed"));
    BOOST_CHECK(key1.SignCompact(hashMsg, detsig));
    BOOST_CHECK(key1C.SignCompact(hashMsg, detsigc));
    BOOST_CHECK(detsig == ParseHex("1bbd4c7468cd9f2ad8169e283598d91f5984f6c0bdf7478e21d7d706f032b9299f08c8ba5f1e080bf5f4a5843da547a952732b519a5cbe359c4eed9a2f7e921004"));
    BOOST_CHECK(detsigc == ParseHex("1fbd4c7468cd9f2ad8169e283598d91f5984f6c0bdf7478e21d7d706f032b9299f08c8ba5f1e080bf5f4a5843da547a952732b519a5cbe359c4eed9a2f7e921004"));
    BOOST_CHECK(key2.SignCompact(hashMsg, detsig));
    BOOST_CHECK(key2C.SignCompact(hashMsg, detsigc));
    BOOST_CHECK(detsig == ParseHex("1cd23ddc1c161710fe2c00112bda306aebbff3a820361ecbfa2f2819de03978a805af44232b128915ec35720fc1708614cb1fda93cf6cb8d6b7cca67213b25af2c"));
    BOOST_CHECK(detsigc == ParseHex("20d23ddc1c161710fe2c00112bda306aebbff3a820361ecbfa2f2819de03978a805af44232b128915ec35720fc1708614cb1fda93cf6cb8d6b7cca67213b25af2c"));
}

BOOST_AUTO_TEST_CASE(key_signature_tests)
{
    // When entropy is specified, we should see at least one high R signature within 20 signatures
    CKey key = DecodeSecret(strSecret1);
    std::string msg = "A message to be signed";
    uint256 msg_hash = Hash(msg.begin(), msg.end());
    std::vector<unsigned char> sig;
    bool found = false;

    for (int i = 1; i <=20; ++i) {
        sig.clear();
        BOOST_CHECK(key.Sign(msg_hash, sig, false, i));
        found = sig[3] == 0x21 && sig[4] == 0x00;
        if (found) {
            break;
        }
    }
    BOOST_CHECK(found);

    // When entropy is not specified, we should always see low R signatures that are less than 70 bytes in 256 tries
    // We should see at least one signature that is less than 70 bytes.
    found = true;
    bool found_small = false;
    for (int i = 0; i < 256; ++i) {
        sig.clear();
        std::string msg = "A message to be signed" + std::to_string(i);
        msg_hash = Hash(msg.begin(), msg.end());
        BOOST_CHECK(key.Sign(msg_hash, sig));
        found = sig[3] == 0x20;
        BOOST_CHECK(sig.size() <= 70);
        found_small |= sig.size() < 70;
    }
    BOOST_CHECK(found);
    BOOST_CHECK(found_small);
}

BOOST_AUTO_TEST_SUITE_END()
