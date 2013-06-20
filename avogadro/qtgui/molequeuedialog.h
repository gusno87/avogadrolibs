/******************************************************************************

  This source file is part of the Avogadro project.

  Copyright 2013 Kitware, Inc.

  This source code is released under the New BSD License, (the "License").

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

******************************************************************************/

#ifndef AVOGADRO_QTGUI_MOLEQUEUEDIALOG_H
#define AVOGADRO_QTGUI_MOLEQUEUEDIALOG_H

#include <QtGui/QDialog>
#include "avogadroqtguiexport.h"

#include <avogadro/qtgui/molequeuewidget.h>

#include <avogadro/core/avogadrocore.h>

#include <molequeue/client/jobobject.h>

namespace Avogadro {
namespace QtGui {

namespace Ui {
class MoleQueueDialog;
}

/**
 * @class MoleQueueDialog molequeuedialog.h <avogadro/qtgui/molequeuedialog.h>
 * @brief The MoleQueueDialog class provides a thin wrapper around
 * MoleQueueWidget for standalone use.
 * @sa MoleQueueWidget MoleQueueManager
 */
class AVOGADROQTGUI_EXPORT MoleQueueDialog : public QDialog
{
  Q_OBJECT
public:
  explicit MoleQueueDialog(QWidget *parent_ = 0);
  ~MoleQueueDialog() AVO_OVERRIDE;

  /**
   * @brief Options controlling job submission behavior in the submitJob method.
   */
  enum SubmitOption {
    /**
     * Keep the dialog open until MoleQueue replies to the submission request.
     * If a submission error occurs, the user will have to opportunity to fix
     * it.
     */
    WaitForSubmissionResponse = 0x1,

    /**
     * Use the program in the template job to initialize the queue/program view.
     * All queues containing a matching program will be expanded, and the first
     * match will be selected.
     * To match, an existing program must contain the template program string,
     * and comparisons are case insensitive.
     */
    SelectProgramFromTemplate = 0x2
  };
  Q_DECLARE_FLAGS(SubmitOptions, SubmitOption)

  /**
   * @brief Return values from submitJob indicating result.
   */
  enum SubmitStatus {
    /**
     * The job was accepted by MoleQueue.
     * This can only be returned when WaitForSubmissionResponse IS set as an
     * option.
     */
    SubmissionSuccessful = 0,

    /**
     * The job was not submitted to MoleQueue, likely due to a disconnected
     * server.
     * This can only be returned when WaitForSubmissionResponse IS NOT
     * set as an option.
     */
    SubmissionFailed,

    /**
     * The job was submitted to MoleQueue. This can only be returned when
     * WaitForSubmissionResponse is NOT set as an option.
     */
    SubmissionAttempted,

    /**
     * The user canceled the submission.
     */
    SubmissionAborted
  };

  /**
   * Show a job configuration dialog and let the user submit the job to
   * MoleQueue.
   * @param parent_ The parent widget for parenting/layout purposes.
   * @param caption The dialog title.
   * @param jobTemplate A template job, used to initialize GUI options.
   * @param options Bitwise combination of flags that control dialog behavior.
   * @param moleQueueId If not NULL, the variable referenced by this pointer
   * will be overwritten by the MoleQueue Id of the submitted job when the
   * option WaitForSubmissionResponse is set.
   * If an error occurs or the required option is not set, this value will be
   * set to MoleQueueWidget::InvalidMoleQueueId.
   * @param submissionRequestId If not NULL, the variable referenced by this
   * pointer will be overwritten by the submitJob JSON-RPC 2.0 request id.
   * If an error occurs, this value will be set to -1.
   * @return A SubmitStatus enum value indicating the result of the submission.
   */
  static SubmitStatus submitJob(QWidget *parent_, const QString &caption,
                                const MoleQueue::JobObject &jobTemplate,
                                SubmitOptions options,
                                unsigned int *moleQueueId = NULL,
                                int *submissionRequestId = NULL);

  /**
   * @return A reference to the internal MoleQueueWidget instance.
   * @{
   */
  MoleQueueWidget &widget();
  const MoleQueueWidget &widget() const;
  /** @} */

  /**
   * Wait for the submitJob reply from MoleQueue. This must be called after
   * MoleQueueDialog.widget().submitJobRequest().
   * @param msTimeout Timeout in milliseconds.
   * @return True if a reply was received, false on timeout.
   */
  bool waitForReply(int msTimeout = 5000) const;

private:
  Ui::MoleQueueDialog *m_ui;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(MoleQueueDialog::SubmitOptions)

} // namespace QtGui
} // namespace Avogadro
#endif // AVOGADRO_QTGUI_MOLEQUEUEDIALOG_H