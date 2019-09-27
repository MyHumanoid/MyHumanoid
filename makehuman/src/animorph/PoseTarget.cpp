#include "animorph/PoseTarget.h"

#include <cstdio>
#ifdef DEBUG
#include <StopClock/StopClock.h>
#endif

using namespace std;
using namespace Animorph;

namespace Animorph
{

bool PoseTarget::load()
{
	const string positive_rotation_type(".rot");
	const string negative_rotation_type("-.rot");
	const string positive_translation_type(".target");
	const string negative_translation_type("-.target");

	DirectoryList dir_list;

	dir_list.setRootPath(fullPath);
	dir_list.setRecursive(0);

	const StringList &str_list(dir_list.getDirectoryList());

	for (StringList::const_iterator sl_it = str_list.begin();
	     sl_it != str_list.end(); sl_it++) {
		const string &file(*sl_it);
		string target_name(file);

		target_name.erase(0, fullPath.length() + 1);

		if (target_name.find(negative_rotation_type) ==
		    target_name.length() - negative_rotation_type.length()) {
			PoseRotation tmpRotation;

			if (tmpRotation.load(file)) {
				tmpRotation.setCat(target_name.substr(0, 2));
				int ret = target_name.find("LIMB");

				if (ret >= 0)
					tmpRotation.setLimb(true);
				else
					tmpRotation.setLimb(false);

				negativeRotations.push_back(tmpRotation);

				modVertex.insert(tmpRotation.getModVertex().begin(),
				                 tmpRotation.getModVertex().end());
				negative = true;

				minAngle = (tmpRotation.getMinAngle() < minAngle)
				               ? tmpRotation.getMinAngle()
				               : minAngle;
			}
		} else if (target_name.find(positive_rotation_type) ==
		           target_name.length() - positive_rotation_type.length()) {
			PoseRotation tmpRotation;

			if (tmpRotation.load(file)) {
				tmpRotation.setCat(target_name.substr(0, 2));
				int ret = target_name.find("LIMB");

				if (ret >= 0)
					tmpRotation.setLimb(true);
				else
					tmpRotation.setLimb(false);

				positiveRotations.push_back(tmpRotation);

				modVertex.insert(tmpRotation.getModVertex().begin(),
				                 tmpRotation.getModVertex().end());
				positive = true;
				maxAngle = (tmpRotation.getMaxAngle() > maxAngle)
				               ? tmpRotation.getMaxAngle()
				               : maxAngle;
			}
		} else if (target_name.find(negative_translation_type) ==
		           target_name.length() - negative_translation_type.length()) {
			PoseTranslation tmpTranslation;
			if (tmpTranslation.load(file)) {
				tmpTranslation.setCat(target_name.substr(0, 2));
				negativeTranslations.push_back(tmpTranslation);
				modVertex.insert(tmpTranslation.getModVertex().begin(),
				                 tmpTranslation.getModVertex().end());

				minAngle = (tmpTranslation.getMinAngle() < minAngle)
				               ? tmpTranslation.getMinAngle()
				               : minAngle;
			}
		} else if (target_name.find(positive_translation_type) ==
		           target_name.length() - positive_translation_type.length()) {
			PoseTranslation tmpTranslation;
			if (tmpTranslation.load(file)) {
				tmpTranslation.setCat(target_name.substr(0, 2));
				positiveTranslations.push_back(tmpTranslation);
				modVertex.insert(tmpTranslation.getModVertex().begin(),
				                 tmpTranslation.getModVertex().end());

				maxAngle = (tmpTranslation.getMaxAngle() > maxAngle)
				               ? tmpTranslation.getMaxAngle()
				               : maxAngle;
			}
		}
	}
	return true; // TODO:verify if considering exceptions is required.
}

void PoseTarget::calcTranslationsFormFactors(
    const VertexVector &vertexvector, PoseTranslationVector &translations)
{
	for (list<PoseTranslation>::iterator translations_it = translations.begin();
	     translations_it != translations.end(); translations_it++) {
		(*translations_it).calcFormFactor(vertexvector);
	}
}

void PoseTarget::calcTranslationsFormFactors(const VertexVector &vertexvector)
{
	if (positive) {
		calcTranslationsFormFactors(vertexvector, positiveTranslations);
	}

	if (negative) {
		calcTranslationsFormFactors(vertexvector, negativeTranslations);
	}
}

void PoseTarget::calcRotationsCenteroids(const VertexVector &vertexvector,
                                         PoseRotationVector &rotations)
{
	for (list<PoseRotation>::iterator rotations_it = rotations.begin();
	     rotations_it != rotations.end(); rotations_it++) {
		// cout << calcCenteroid((*rotations_it).getCenterVertexNumbers(),
		// vertexvector) << endl;
		(*rotations_it)
		    .setCenter(calcCenteroid((*rotations_it).getCenterVertexNumbers(),
		                             vertexvector));
	}
}

const Vector3f PoseTarget::getFirstRotationCenteroid()
{
	Vector3f res(0.f, 0.f, 0.f);

	if (positive) {
		for (list<PoseRotation>::iterator rotations_it = positiveRotations.begin();
		     rotations_it != positiveRotations.end(); rotations_it++) {
			// cout << calcCenteroid((*rotations_it).getCenterVertexNumbers(),
			// vertexvector) << endl;
			if ((*rotations_it).getLimb() == false)
				continue;
			return res = (*rotations_it).getCenter();
		}
	}

	if (negative) {
		for (list<PoseRotation>::iterator rotations_it = negativeRotations.begin();
		     rotations_it != negativeRotations.end(); rotations_it++) {
			// cout << calcCenteroid((*rotations_it).getCenterVertexNumbers(),
			// vertexvector) << endl;
			if ((*rotations_it).getLimb() == false)
				continue;
			return res = (*rotations_it).getCenter();
		}
	}
	return res;
}

void PoseTarget::calcNormalizations()
{
	if (normalizationInited)
		return;

	list<PoseRotation>::iterator rotations_it;
	list<PoseTranslation>::iterator translations_it;

	for (rotations_it = positiveRotations.begin();
	     rotations_it != positiveRotations.end(); rotations_it++) {
		if ((*rotations_it).getMinAngle() != minAngle ||
		    (*rotations_it).getMaxAngle() != maxAngle) {
			(*rotations_it).setNormalize(true);
		}
	}

	for (rotations_it = negativeRotations.begin();
	     rotations_it != negativeRotations.end(); rotations_it++) {
		if ((*rotations_it).getMinAngle() != minAngle ||
		    (*rotations_it).getMaxAngle() != maxAngle) {
			(*rotations_it).setNormalize(true);
		}
	}

	for (translations_it = positiveTranslations.begin();
	     translations_it != positiveTranslations.end(); translations_it++) {
		if ((*translations_it).getMinAngle() != minAngle ||
		    (*translations_it).getMaxAngle() != maxAngle) {
			(*translations_it).setNormalize(true);
		}
	}

	for (translations_it = negativeTranslations.begin();
	     translations_it != negativeTranslations.end(); translations_it++) {
		if ((*translations_it).getMinAngle() != minAngle ||
		    (*translations_it).getMaxAngle() != maxAngle) {
			(*translations_it).setNormalize(true);
		}
	}

	normalizationInited = true;
}

void PoseTarget::calcRotationsCenteroids(const VertexVector &vertexvector)
{
	if (positive) {
		calcRotationsCenteroids(vertexvector, positiveRotations);
	}

	if (negative) {
		calcRotationsCenteroids(vertexvector, negativeRotations);
	}
}

} // namespace Animorph
