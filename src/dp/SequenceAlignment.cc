#include "dp/SequenceAlignment.h"

#include <algorithm>
#include <vector>

namespace guozi::dp
{

SequenceAlignmentResult
NeedlemanWunsch(const std::string &seq1, const std::string &seq2, int64_t mismatchPenalty, int64_t gapPenalty)
{
	auto len1 = seq1.size();
	auto len2 = seq2.size();
	std::vector<std::vector<int64_t>> penalties(len1 + 1, std::vector<int64_t>(len2 + 1, 0));
	for (size_t i = 0; i <= len1; ++i)
	{
		penalties[i][0] = i * gapPenalty;
	}

	for (size_t j = 0; j <= len2; ++j)
	{
		penalties[0][j] = j * gapPenalty;
	}

	for (size_t i = 1; i <= len1; ++i)
	{
		for (size_t j = 1; j <= len2; ++j)
		{
			int64_t matchScore = (seq1[i - 1] == seq2[j - 1]) ? 0 : mismatchPenalty;
			penalties[i][j] = std::min(penalties[i - 1][j - 1] + matchScore,
									   std::min(penalties[i - 1][j] + gapPenalty, penalties[i][j - 1] + gapPenalty));
		}
	}

	std::string alignedSeq1;
	std::string alignedSeq2;
	for (size_t i = len1, j = len2; i > 0 || j > 0;)
	{
		if (i > 0 && j > 0 &&
			penalties[i][j] == penalties[i - 1][j - 1] + ((seq1[i - 1] == seq2[j - 1]) ? 0 : mismatchPenalty))
		{
			alignedSeq1.push_back(seq1[i - 1]);
			alignedSeq2.push_back(seq2[j - 1]);

			i--;
			j--;
		}
		else if (i > 0 && (j == 0 || penalties[i][j] == penalties[i - 1][j] + gapPenalty))
		{
			alignedSeq1.push_back(seq1[i - 1]);
			alignedSeq2.push_back('-');

			i--;
		}
		else
		{
			alignedSeq1.push_back('-');
			alignedSeq2.push_back(seq2[j - 1]);

			j--;
		}
	}

	std::reverse(alignedSeq1.begin(), alignedSeq1.end());
	std::reverse(alignedSeq2.begin(), alignedSeq2.end());

	return SequenceAlignmentResult(alignedSeq1, alignedSeq2, penalties[len1][len2]);
}

} // namespace guozi::dp
