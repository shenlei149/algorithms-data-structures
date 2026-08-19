#include "dp/SequenceAlignment.h"

#include <gtest/gtest.h>

#include <cstdint>
#include <string>

namespace
{

using guozi::dp::NeedlemanWunsch;
using guozi::dp::SequenceAlignmentResult;

void ExpectValidAlignment(const std::string &seq1, const std::string &seq2,
						  int64_t mismatchPenalty, int64_t gapPenalty,
						  const SequenceAlignmentResult &result)
{
	ASSERT_EQ(result.AlignedSeq1().size(), result.AlignedSeq2().size());

	std::string ungappedSeq1;
	std::string ungappedSeq2;
	int64_t score = 0;
	for (size_t i = 0; i < result.AlignedSeq1().size(); ++i)
	{
		const char alignedChar1 = result.AlignedSeq1()[i];
		const char alignedChar2 = result.AlignedSeq2()[i];
		if (alignedChar1 == '-')
		{
			ASSERT_NE(alignedChar2, '-');
			ungappedSeq2.push_back(alignedChar2);
			score += gapPenalty;
		}
		else if (alignedChar2 == '-')
		{
			ungappedSeq1.push_back(alignedChar1);
			score += gapPenalty;
		}
		else
		{
			ungappedSeq1.push_back(alignedChar1);
			ungappedSeq2.push_back(alignedChar2);
			if (alignedChar1 != alignedChar2)
			{
				score += mismatchPenalty;
			}
		}
	}

	EXPECT_EQ(seq1, ungappedSeq1);
	EXPECT_EQ(seq2, ungappedSeq2);
	EXPECT_EQ(score, result.Score());
}

TEST(SequenceAlignmentTest, ProvidedExampleFindsOptimalAlignment)
{
	const auto result = NeedlemanWunsch("AGTACG", "ACATAG", 2, 1);

	EXPECT_EQ("A-GTACG", result.AlignedSeq1());
	EXPECT_EQ("ACATA-G", result.AlignedSeq2());
	EXPECT_EQ(4, result.Score());
	ExpectValidAlignment("AGTACG", "ACATAG", 2, 1, result);
}

TEST(SequenceAlignmentTest, EmptySequencesHaveZeroScore)
{
	const auto result = NeedlemanWunsch("", "", 2, 1);

	EXPECT_EQ("", result.AlignedSeq1());
	EXPECT_EQ("", result.AlignedSeq2());
	EXPECT_EQ(0, result.Score());
}

TEST(SequenceAlignmentTest, EmptyFirstSequenceUsesGaps)
{
	const auto result = NeedlemanWunsch("", "ACGT", 2, 3);

	EXPECT_EQ("----", result.AlignedSeq1());
	EXPECT_EQ("ACGT", result.AlignedSeq2());
	EXPECT_EQ(12, result.Score());
	ExpectValidAlignment("", "ACGT", 2, 3, result);
}

TEST(SequenceAlignmentTest, EmptySecondSequenceUsesGaps)
{
	const auto result = NeedlemanWunsch("ACGT", "", 2, 3);

	EXPECT_EQ("ACGT", result.AlignedSeq1());
	EXPECT_EQ("----", result.AlignedSeq2());
	EXPECT_EQ(12, result.Score());
	ExpectValidAlignment("ACGT", "", 2, 3, result);
}

TEST(SequenceAlignmentTest, IdenticalSequencesHaveZeroScoreAndNoGaps)
{
	const auto result = NeedlemanWunsch("ACGT", "ACGT", 2, 1);

	EXPECT_EQ("ACGT", result.AlignedSeq1());
	EXPECT_EQ("ACGT", result.AlignedSeq2());
	EXPECT_EQ(0, result.Score());
}

TEST(SequenceAlignmentTest, MismatchCanBeCheaperThanTwoGaps)
{
	const auto result = NeedlemanWunsch("A", "G", 2, 5);

	EXPECT_EQ("A", result.AlignedSeq1());
	EXPECT_EQ("G", result.AlignedSeq2());
	EXPECT_EQ(2, result.Score());
}

TEST(SequenceAlignmentTest, TwoGapsCanBeCheaperThanMismatch)
{
	const auto result = NeedlemanWunsch("A", "G", 5, 1);

	EXPECT_EQ("-A", result.AlignedSeq1());
	EXPECT_EQ("G-", result.AlignedSeq2());
	EXPECT_EQ(2, result.Score());
	ExpectValidAlignment("A", "G", 5, 1, result);
}

TEST(SequenceAlignmentTest, HandlesPrefixAndSuffixGaps)
{
	const auto result = NeedlemanWunsch("AC", "TACG", 3, 1);

	EXPECT_EQ("-AC-", result.AlignedSeq1());
	EXPECT_EQ("TACG", result.AlignedSeq2());
	EXPECT_EQ(2, result.Score());
	ExpectValidAlignment("AC", "TACG", 3, 1, result);
}

TEST(SequenceAlignmentTest, AlignmentIsSymmetric)
{
	const auto forward = NeedlemanWunsch("GATTACA", "GCATGCU", 2, 1);
	const auto reverse = NeedlemanWunsch("GCATGCU", "GATTACA", 2, 1);

	EXPECT_EQ("G-ATTACA", forward.AlignedSeq1());
	EXPECT_EQ("GCA-TGCU", forward.AlignedSeq2());
	EXPECT_EQ("GCA-TGCU", reverse.AlignedSeq1());
	EXPECT_EQ("G-ATTACA", reverse.AlignedSeq2());
	EXPECT_EQ(forward.Score(), reverse.Score());
	ExpectValidAlignment("GATTACA", "GCATGCU", 2, 1, forward);
	ExpectValidAlignment("GCATGCU", "GATTACA", 2, 1, reverse);
}

TEST(SequenceAlignmentTest, HandlesZeroPenalties)
{
	const auto result = NeedlemanWunsch("AC", "GT", 0, 0);

	EXPECT_EQ("AC", result.AlignedSeq1());
	EXPECT_EQ("GT", result.AlignedSeq2());
	EXPECT_EQ(0, result.Score());
	ExpectValidAlignment("AC", "GT", 0, 0, result);
}

} // namespace
