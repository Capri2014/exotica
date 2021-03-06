//
// Copyright (c) 2018, University of Edinburgh
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//  * Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of  nor the names of its contributors may be used to
//    endorse or promote products derived from this software without specific
//    prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#ifndef EXOTICA_CORE_PLANNING_PROBLEM_H_
#define EXOTICA_CORE_PLANNING_PROBLEM_H_

#include <chrono>
#include <map>
#include <string>
#include <vector>

#include <exotica_core/object.h>
#include <exotica_core/scene.h>
#include <exotica_core/task_map.h>
#include <exotica_core/task_space_vector.h>
#include <exotica_core/tools/conversions.h>
#include <exotica_core/tools/uncopyable.h>

#define REGISTER_PROBLEM_TYPE(TYPE, DERIV) EXOTICA_CORE_REGISTER_CORE(exotica::PlanningProblem, TYPE, DERIV)

namespace exotica
{
enum class TerminationCriterion
{
    NotStarted = -1,
    // Continue = 0,
    IterationLimit,
    BacktrackIterationLimit,
    StepTolerance,
    FunctionTolerance,
    GradientTolerance,
    Divergence,
    UserDefined
    // Condition,
};

class PlanningProblem : public Object, Uncopyable, public virtual InstantiableBase, public std::enable_shared_from_this<PlanningProblem>
{
public:
    PlanningProblem();
    virtual ~PlanningProblem() = default;
    void InstantiateBase(const Initializer& init) override;
    TaskMapMap& GetTaskMaps();
    TaskMapVec& GetTasks();
    ScenePtr GetScene();
    std::string Print(std::string prepend) override;
    void SetStartState(Eigen::VectorXdRefConst x);
    void SetStartTime(double t);
    Eigen::VectorXd GetStartState();
    double GetStartTime();
    Eigen::VectorXd ApplyStartState(bool update_traj = true);
    virtual void PreUpdate();
    unsigned int GetNumberOfProblemUpdates() { return number_of_problem_updates_; }
    void ResetNumberOfProblemUpdates() { number_of_problem_updates_ = 0; }
    std::pair<std::vector<double>, std::vector<double>> GetCostEvolution();
    double GetCostEvolution(int index);
    void ResetCostEvolution(unsigned int size);
    void SetCostEvolution(int index, double value);
    KinematicRequestFlags GetFlags() { return flags_; }
    /// \brief Evaluates whether the problem is valid.
    virtual bool IsValid() { ThrowNamed("Not implemented"); };
    int N;
    double t_start;
    TerminationCriterion termination_criterion;

protected:
    void UpdateTaskKinematics(std::shared_ptr<KinematicResponse> response);
    void updateMultipleTaskKinematics(std::vector<std::shared_ptr<KinematicResponse>> responses);

    ScenePtr scene_;
    TaskMapMap task_maps_;
    TaskMapVec tasks_;
    KinematicRequestFlags flags_;
    Eigen::VectorXd start_state_;
    unsigned int number_of_problem_updates_ = 0;  // Stores number of times the problem has been updated
    std::vector<std::pair<std::chrono::high_resolution_clock::time_point, double>> cost_evolution_;
};

typedef Factory<PlanningProblem> PlanningProblemFac;
typedef std::shared_ptr<PlanningProblem> PlanningProblemPtr;
}

#endif  // EXOTICA_CORE_PLANNING_PROBLEM_H_
