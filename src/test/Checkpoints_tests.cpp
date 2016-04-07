// Copyright (c) 2011-2013 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Unit tests for block-chain checkpoints
//

#include "checkpoints.h"

#include "uint256.h"
#include "test/test_bitcoin.h"
#include "chainparams.h"

#include <boost/test/unit_test.hpp>

using namespace std;

BOOST_FIXTURE_TEST_SUITE(Checkpoints_tests, BasicTestingSetup)

BOOST_AUTO_TEST_CASE(sanity)
{
    const Checkpoints::CCheckpointData& checkpoints = Params(CBaseChainParams::MAIN).Checkpoints();
    uint256 p740001 = uint256S("0x00000000000002c7aaa3ad688eb5af7d2779f4aa1d9a1b9a5c3cfb4ccad56caa");
    uint256 p1402167 = uint256S("0x8215cd9feb2db9bf74514361a9d4b4440f054101079df8bb6bf629065db3ab29");
    BOOST_CHECK(Checkpoints::CheckBlock(checkpoints, 740001, p740001));
    BOOST_CHECK(Checkpoints::CheckBlock(checkpoints, 1402167, p1402167));

    
    // Wrong hashes at checkpoints should fail:
    BOOST_CHECK(!Checkpoints::CheckBlock(checkpoints, 740001, p1402167));
    BOOST_CHECK(!Checkpoints::CheckBlock(checkpoints, 1402167, p740001));

    // ... but any hash not at a checkpoint should succeed:
    BOOST_CHECK(Checkpoints::CheckBlock(checkpoints, 740001+1, p1402167));
    BOOST_CHECK(Checkpoints::CheckBlock(checkpoints, 1402167+1, p740001));

    BOOST_CHECK(Checkpoints::GetTotalBlocksEstimate(checkpoints) >= 1402167);
}    

BOOST_AUTO_TEST_SUITE_END()
