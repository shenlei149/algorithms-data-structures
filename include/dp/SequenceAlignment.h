#pragma once

#include <cstdint>
#include <string>

namespace guozi::dp
{

class SequenceAlignmentResult
{
public:
	SequenceAlignmentResult(std::string seq1, std::string seq2, int64_t score)
		: alignedSeq1_(std::move(seq1))
		, alignedSeq2_(std::move(seq2))
		, score_(score)
	{}

	[[nodiscard]] const std::string &AlignedSeq1() const { return alignedSeq1_; }

	[[nodiscard]] const std::string &AlignedSeq2() const { return alignedSeq2_; }

	[[nodiscard]] int64_t Score() const { return score_; }

private:
	std::string alignedSeq1_;
	std::string alignedSeq2_;
	int64_t score_;
};

// suppose seq contains only A, C, G, T
SequenceAlignmentResult
NeedlemanWunsch(const std::string &seq1, const std::string &seq2, int64_t mismatchPenalty, int64_t gapPenalty);

} // namespace guozi::dp
